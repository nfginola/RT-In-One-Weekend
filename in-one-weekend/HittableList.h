#pragma once
#include "Hittable.h"

class HittableList
{
public:
	void add(std::unique_ptr<Hittable> hittable);
	void clear() { m_hittables.clear(); }
	std::optional<HitRecord> intersects(const Ray& r) const;

private:
	std::vector<std::unique_ptr<Hittable>> m_hittables;

};

