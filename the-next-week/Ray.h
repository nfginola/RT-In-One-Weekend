#pragma once
#include "Vec3.h"

class Ray
{
public:
	Ray(const Vec3& origin, const Vec3& direction, f32 t_min = 0.001f, f32 t_max = std::numeric_limits<f32>::max()) :
		m_origin(origin),
		m_direction(direction),
		m_t_min(t_min),
		m_t_max(t_max) 
	{
		m_direction.normalized();
	}
	
	const Vec3& origin() const { return m_origin; }
	const Vec3& direction() const { return m_direction; }
	Vec3 solve(f32 t) const { return m_origin + m_direction * t; }
	f32 t_min() const { return m_t_min; }
	f32 t_max() const { return m_t_max; }

private:
	Vec3 m_origin;
	Vec3 m_direction;
	f32 m_t_min{ 0.f };
	f32 m_t_max{ 0.f };
};

