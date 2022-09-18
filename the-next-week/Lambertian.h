#pragma once
#include "Material.h"

class Lambertian : public Material
{
public:
	Lambertian(const Color& color) : m_albedo(color) {}

	// Always scatters
	std::optional<MaterialRecord> scatter(const Ray& r, const HitRecord& hit_rec);

private:
	Color m_albedo;
};

