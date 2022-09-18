#pragma once
#include "Vec3.h"

class Material;

struct HitRecord
{
	f32 t{ 0.f };
	Vec3 hit_point;
	Vec3 normal;
	bool front_face{ true };
	std::shared_ptr<Material> material;

	// Resolves the normal and face depending on whether we are looking from inside or outside the geometry
	void resolve_normal_and_face(const Vec3& ray_dir, const Vec3& outward_normal)
	{
		front_face = dot(ray_dir, outward_normal) < 0.f;
		normal = front_face ? outward_normal : -outward_normal;
	}
};
