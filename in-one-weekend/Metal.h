#pragma once
#include "Material.h"

class Metal : public Material
{
public:
	Metal(const Color& color, f32 fuzz) : m_albedo(color), m_fuzz(fuzz < 1.f ? fuzz : 1.f) {}

	// Scatters sometimes
	std::optional<MaterialRecord> scatter(const Ray& r, const HitRecord& hit_rec);

private:
	Color m_albedo;
	f32 m_fuzz{ 0.f };	// 0.0: no perturbation
};

