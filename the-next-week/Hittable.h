#pragma once
#include <optional>
#include "HitRecord.h"
#include "Ray.h"

class Hittable
{
public:
	virtual std::optional<HitRecord> intersects(const Ray& r) const = 0;

	virtual ~Hittable() {}

};


