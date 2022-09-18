#pragma once
#include "Ray.h"

class Camera
{
public:
	Camera(const Point3& look_from, const Point3& look_at, const Vec3& up_dir, 
		f32 aperture, f32 focus_dist,
		f32 vertical_fov = 90.f, f32 aspect_ratio = 16.f / 9.f);

	Ray get_ray(f32 u, f32 v) const;

private:
	Point3 m_origin;
	Point3 m_vplane_lower_left;
	Vec3 m_horiz;
	Vec3 m_vert;
	f32 m_lens_radius{ 0.f };

	Vec3 m_u;
	Vec3 m_v;
	Vec3 m_w;

};

