#pragma once
#include "Material.h"
class Dielectrics : public Material
{
public:
	Dielectrics(f32 index_of_refraction) : m_refr_idx(index_of_refraction) {}
	std::optional<MaterialRecord> scatter(const Ray& r, const HitRecord& hit_rec);

private:
	f32 m_refr_idx{ 0.f };

private:
	static f32 reflectance(f32 cosine, f32 refl_idx);

};

