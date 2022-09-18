#include "Sphere.h"
#include "Ray.h"

std::optional<HitRecord> Sphere::intersects(const Ray& r) const
{
	const Vec3& ray_dir = r.direction();
	const Vec3 oc = r.origin() - m_center;
	const f32 a = dot(ray_dir, ray_dir);
	const f32 h = dot(ray_dir, oc);
	const f32 c = dot(oc, oc) - std::powf(m_radius, 2.0);
	const f32 discr = h * h - a * c;

	if (discr < 0.f)
		return {};

	// Precompute sqrtf
	const f32 discr_sqrtf = sqrtf(discr);

	// Evaluate if solution is within valid bounds
	const auto solution_valid = [](const Ray& r, f32 t) -> bool
	{
		const bool behind = t < r.t_min();
		const bool too_far = t > r.t_max();
		return !(behind || too_far);
	};

	// Get the nearest valid root (start with '-')
	f32 t = (-h - discr_sqrtf) / a;
	if (!solution_valid(r, t))
	{
		t = (-h + discr_sqrtf) / a;
		if (!solution_valid(r, t))
			return {};
	}

	// Resolve hit point
	const Vec3 hit_point = r.solve(t);
	
	HitRecord rec{};
	rec.t = t;
	rec.hit_point = hit_point;
	rec.material = m_material;

	// Resolve normal and face
	Vec3 outward_normal = (hit_point - m_center) / m_radius;
	// Observe division by radius --> Negative radius will invert the normal

	rec.resolve_normal_and_face(ray_dir, outward_normal);



	return rec;
}
