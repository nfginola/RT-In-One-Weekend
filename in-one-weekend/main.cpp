#include <iostream>
#include "Common.h"
#include "Ray.h"
#include "ImagePPM.h"
#include "Sphere.h"
#include "HittableList.h"
#include "Camera.h"
#include "Utils.h"
#include "Lambertian.h"
#include "Metal.h"
#include "Dielectrics.h"

#include "Stopwatch.h"

#include <array>
#include <numeric>
#include <omp.h>

#include "DiffuseScatterDefines.h"

Color ray_color(const Ray& r, const HittableList& world, u32 depth);
Color resolve_color(const Color& color, u16 spp);

HittableList random_scene();

int main()
{	
	/*
		RH coordinate system. -Z inwards
		UV (0,0) at bottom left. 
	
		RT scene
	*/
	constexpr f32 ASPECT_RATIO{ 3.f / 2.f };
	constexpr u32 WIDTH{ 1200 };
	constexpr u32 HEIGHT{ (u32)(WIDTH * 1.f / ASPECT_RATIO) };
	constexpr u16 SPP = 500;
	constexpr u32 MAX_DEPTH = 50;
	auto image = ImagePPM("_image.ppm", WIDTH, HEIGHT);
	
	auto world = random_scene();

	const Point3 look_from(13.f, 2.f, 3.f);
	const Point3 look_at(0.f);
	const Vec3 up_dir(0.f, 1.f, 0.f);
	const f32 dist_to_focus = 10.f;
	const f32 aperture = 0.1f;
	const f32 vert_fov = 20.f;

	const Camera cam(look_from, look_at, up_dir, aperture, dist_to_focus, vert_fov, ASPECT_RATIO);

	std::vector<Color> colors;
	colors.resize(HEIGHT * WIDTH);
	i32 idx = 0;

	Stopwatch sw_program;
	sw_program.start();

	for (i32 y = HEIGHT-1; y >= 0; --y)
	{
		std::cout << "\rScanlines remaining: " << y << ' ' << std::flush;
		for (i32 x = 0; x < WIDTH; ++x)
		{
			Stopwatch sw;
			sw.start();

			std::array<Color, SPP> samples;
#pragma omp parallel num_threads(8)
			{
#pragma omp for schedule(dynamic, SPP / 64)		// Better --> "different iterations in the loop may take different time to execute"
				for (int sample = 0; sample < SPP; ++sample)
				{
					const f32 u = (f32)(x + random_float()) / (WIDTH - 1);
					const f32 v = (f32)(y + random_float()) / (HEIGHT - 1);
					const Ray r = cam.get_ray(u, v);
					samples[sample] = ray_color(r, world, MAX_DEPTH);
				}
			}
			
			sw.stop();
				
			Color color = std::accumulate(samples.begin(), samples.end(), Vec3(0.f));
			color = resolve_color(color, SPP);

			colors[idx] = color;
			++idx;
		}
	}

	for (int i = 0; i < colors.size(); ++i)
		image.write_color(colors[i].x, colors[i].y, colors[i].z);

	sw_program.stop();


	std::cout << "\n";
	std::cout << "Done!\n";
	std::cout << "Program elapsed: " << sw_program.elapsed(Stopwatch::Unit::eMillisecond) << " (ms)\n";

	return 0;
}

Color resolve_color(const Color& color, u16 spp)
{
	// Resolve multisampling
	const f32 scale = 1.f / spp;
	Vec3 scaled_color = color * scale;

	// Gamma correct (1/~2.0) (1/2.22f is usually used)
	const Vec3 gamma_corrected_color = Vec3(
		sqrtf(scaled_color.x),
		sqrtf(scaled_color.y),
		sqrtf(scaled_color.z)
	);

	const Vec3 clamped_color = Vec3(
		clamp(gamma_corrected_color.x, 0.f, 0.999f),
		clamp(gamma_corrected_color.y, 0.f, 0.999f),
		clamp(gamma_corrected_color.z, 0.f, 0.999f)
	);

	return clamped_color;
}

Color ray_color(const Ray& r, const HittableList& world, u32 depth)
{
	// No more light is gathered
	if (depth <= 0)	
		return Color(0.f);

	const auto hit_res = world.intersects(r);
	if (hit_res)
	{
		auto scatter_res = hit_res->material->scatter(r, *hit_res);
		if (scatter_res)
			return scatter_res->attenuation * ray_color(scatter_res->scattered_ray, world, depth - 1);

		// Failed to scatter, no light accum.
		return Color(0.f);
	}

	// Color the sky if no hits
	// [-1.0, 1.0] --> [0.0, 1.0]
	const f32 t = 0.5f * (r.direction().y + 1.f);

	const Vec3 sky_color(0.5f, 0.7f, 1.f);
	const Vec3 horizon_color(1.f, 1.f, 1.f);

	return (1.f - t) * horizon_color + t * sky_color;
}

HittableList random_scene()
{
	HittableList world;

	auto ground_material = std::make_shared<Lambertian>(Color(0.5, 0.5, 0.5));
	world.add(std::make_unique<Sphere>(Point3(0.f, -1000.f, 0.f), 1000, ground_material));

	for (i32 a = -11; a < 11; ++a) 
	{
		for (i32 b = -11; b < 11; ++b) 
		{
			f32 choose_mat = random_float();
			Point3 center(a + 0.9f * random_float(), 0.2f, b + 0.9f * random_float());

			if ((center - Point3(4.f, 0.2f, 0.f)).length() > 0.9f) {
				std::shared_ptr<Material> sphere_material;

				if (choose_mat < 0.8) 
				{
					// diffuse
					//auto albedo = color::random() * color::random();
					const Color albedo =
						Color(random_float(), random_float(), random_float()) *
						Color(random_float(), random_float(), random_float());
					sphere_material = std::make_shared<Lambertian>(albedo);
					world.add(std::make_unique<Sphere>(center, 0.2f, sphere_material));
				}
				else if (choose_mat < 0.95) 
				{
					// metal
					const Color albedo = Color(random_float(0.5f, 1.f), random_float(0.5f, 1.f), random_float(0.5f, 1.f));
					f32 fuzz = random_float(0.f, 0.5f);
					sphere_material = std::make_shared<Metal>(albedo, fuzz);
					world.add(std::make_unique<Sphere>(center, 0.2f, sphere_material));
				}
				else 
				{
					// glass
					sphere_material = std::make_shared<Dielectrics>(1.5f);
					world.add(std::make_unique<Sphere>(center, 0.2f, sphere_material));
				}
			}
		}
	}

	auto material1 = std::make_shared<Dielectrics>(1.5f);
	world.add(std::make_unique<Sphere>(Point3(0.f, 1.f, 0.f), 1.f, material1));

	auto material2 = std::make_shared<Lambertian>(Color(0.4f, 0.2f, 0.1f));
	world.add(std::make_unique<Sphere>(Point3(-4.f, 1.f, 0.f), 1.f, material2));

	auto material3 = std::make_shared<Metal>(Color(0.7f, 0.6f, 0.5f), 0.f);
	world.add(std::make_unique<Sphere>(Point3(4.f, 1.f, 0.f), 1.f, material3));

	return world;
}