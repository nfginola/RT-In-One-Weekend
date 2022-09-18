#include "Lambertian.h"
#include "HitRecord.h"
#include "DiffuseScatterDefines.h"
#include "Utils.h"

std::optional<MaterialRecord> Lambertian::scatter(const Ray& r, const HitRecord& hit_rec)
{
	// 9.3 Modeling Light Scatter & Reflectance

	/*
		Using the rejection method:
			"Sampling random point within unit sphere" produces directions on the hemisphere which high probability close to the normal!"
			"Probability scales with cos(x)^3 where x is angle from the normal"	--> How do we see this?

	*/

#ifdef REJECTION_METHOD
	const Point3 target = hit_rec.hit_point + hit_rec.normal + random_vec_in_unit_sphere();
#elif defined(TRUE_LAMBERTIAN_REFLECTION)
	const Point3 target = hit_rec.hit_point + hit_rec.normal + random_unit_vec();
#elif defined(HEMISPHERICAL_SCATTERING)
	const Point3 target = hit_rec.hit_point + random_in_hemisphere(hit_rec.normal);
#endif
	
	// Handle degenerate scatter direction
	// If random vector is opposite to normal --> sum to 0..
	Vec3 scatter_dir = target - hit_rec.hit_point;
	if (scatter_dir.near_zero())
		scatter_dir = hit_rec.normal;

	return MaterialRecord
	{
		.attenuation = m_albedo,
		.scattered_ray = Ray(hit_rec.hit_point, scatter_dir)
	};
}
