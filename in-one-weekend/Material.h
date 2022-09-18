#pragma once
#include "Ray.h"

struct HitRecord;

struct MaterialRecord
{
	Color attenuation;
	Ray scattered_ray;
};

class Material
{
public:
	virtual std::optional<MaterialRecord> scatter(const Ray& r, const HitRecord& hit_rec) = 0;

	virtual ~Material() {}
};
