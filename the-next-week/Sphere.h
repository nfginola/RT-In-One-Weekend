#pragma once
#include "Hittable.h"
#include "Material.h"

class Sphere : public Hittable
{
public:
	Sphere(const Vec3& center, f32 radius, std::shared_ptr<Material> material) : m_center(center), m_radius(radius), m_material(material) {}
	std::optional<HitRecord> intersects(const Ray& r) const;

private:
	Vec3 m_center;
	f32 m_radius{ 0.f };
	std::shared_ptr<Material> m_material;
};

