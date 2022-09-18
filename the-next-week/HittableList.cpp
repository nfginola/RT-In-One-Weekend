#include "HittableList.h"

void HittableList::add(std::unique_ptr<Hittable> hittable)
{
	m_hittables.push_back(std::move(hittable));
}

std::optional<HitRecord> HittableList::intersects(const Ray& r) const
{
	f32 closest_t{ std::numeric_limits<f32>::max() };
	std::optional<HitRecord> closest_hit;

	for (const auto& hittable : m_hittables)
	{
		auto hit_res = hittable->intersects(r);
		if (hit_res)
		{
			if (hit_res->t < closest_t)
			{
				closest_t = hit_res->t;
				closest_hit = hit_res;
			}
		}
	}

	return closest_hit;
}
