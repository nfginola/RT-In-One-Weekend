#include "Camera.h"
#include "Utils.h"

Camera::Camera(const Point3& look_from, const Point3& look_at, const Vec3& up_dir, 
	f32 aperture, f32 focus_dist, f32 vertical_fov, f32 aspect_ratio)
{
	const f32 theta = to_rad(vertical_fov);
	const f32 h = tanf(theta / 2.f);			// Implicit focal length = 1
	const f32 vp_height = 2.f * h;
	const f32 vp_width = vp_height * aspect_ratio;

	m_w = (look_from - look_at).normalized();	// Local backward
	m_u = cross(-m_w, up_dir).normalized();		// Local right
	m_v = cross(m_u, -m_w).normalized();		// Local up

	m_origin = look_from;
	// Added focus dist here as well --> Projection is just scaling!
	m_horiz = focus_dist * vp_width * m_u;
	m_vert = focus_dist * vp_height * m_v;

	// Origin --> Left --> Down --> Forward (to virtual plane)
	m_vplane_lower_left = m_origin - m_horiz / 2 - m_vert / 2 - m_w * focus_dist;		
	// Focus_dist added for Defocus
	// This pushes the image plane further away

	// Aperture is the diameter of the lens!
	m_lens_radius = aperture / 2.f;
}

Ray Camera::get_ray(f32 s, f32 t) const
{
	// Offset for origin to simulate aperture
	const Vec3 p_in_unit_disk = m_lens_radius * random_in_unit_disk();
	// Orient the disk onto our camera plane
	const Vec3 offset =
		m_u * p_in_unit_disk.x +	// Get pos along U
		m_v * p_in_unit_disk.y;		// Get pos along V

	// Assuming UV (0,0) at bottom left
	const Vec3 p_on_vplane = m_vplane_lower_left + s * m_horiz + t * m_vert;
	const Vec3 dir = p_on_vplane - m_origin;

	return Ray(
		m_origin + offset,	// Get a random position within aperture disk!
		dir - offset);		// Offset the direction so that we are looking at the same point that we would've without the offset.
}
