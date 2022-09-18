#include "Vec3.h"
#include "Utils.h"

Vec3 operator+(const Vec3& lh, const Vec3& rh)
{
	return Vec3(lh.x + rh.x, lh.y + rh.y, lh.z + rh.z);
}

Vec3 operator-(const Vec3& lh, const Vec3& rh)
{
	return Vec3(lh.x - rh.x, lh.y - rh.y, lh.z - rh.z);
}

Vec3 operator*(const Vec3& lh, f32 t)
{
	return Vec3(lh.x * t, lh.y * t, lh.z * t);
}

Vec3 operator*(f32 t, const Vec3& rh)
{
	return rh * t;
}

Vec3 operator*(const Vec3& lh, const Vec3& rh)
{
	return Vec3(lh.x * rh.x, lh.y * rh.y, lh.z * rh.z);
}

Vec3 operator/(const Vec3& lh, f32 t)
{
	return lh * (1.f / t);
}

Vec3 operator/(f32 t, const Vec3& rh)
{
	return rh * (1.f / t);
}

//f32 dot(const Vec3& lh, const Vec3& rh)
//{
//	return lh.x * rh.x + lh.y * rh.y + lh.z * rh.z;
//}
//
//Vec3 cross(const Vec3& lh, const Vec3& rh)
//{
//	return Vec3(
//		lh.y * rh.z - lh.z * rh.y,
//		lh.z * rh.x - lh.x * rh.z,
//		lh.x * rh.y - lh.y * rh.x);
//}
//
//Vec3 random_vec(f32 min, f32 max)
//{
//	return Vec3((f32)random_float(min, max), (f32)random_float(min, max), (f32)random_float(min, max));
//}
//
//Vec3 reflect(const Vec3& incident, const Vec3& normal)
//{
//	return incident + 2.f * dot(-incident, normal) * normal;
//}
//
//Vec3 random_vec_in_unit_sphere()
//{
//	while (true)
//	{
//		const Vec3 p = random_vec(-1.f, 1.f);
//		if (p.length_sq() >= 1.f)
//			continue;
//		return p;
//	}
//}
//
//Vec3 random_unit_vec()
//{
//	return random_vec_in_unit_sphere().normalized();
//}
//
//Vec3 random_in_hemisphere(const Vec3& normal) 
//{
//	Vec3 in_unit_sphere = random_vec_in_unit_sphere();
//	if (dot(in_unit_sphere, normal) > 0.0) // In the same hemisphere as the normal
//		return in_unit_sphere;
//	else
//		return -in_unit_sphere;
//}
//
////Vec3 random_in_unit_disk()
////{
////	while (true)
////	{
////		const Vec3 p = Vec3(random_float(-1.f, 1.f), random_float(-1.f, 1.f), 0.f);
////		if (p.length_sq() >= 1.f)
////			continue;
////		return p;
////	}
////}
//
//Vec3 refract(const Vec3& uv, const Vec3& normal, f32 etai_over_etat)
//{
//	const f32 cos_theta = fmin(dot(-uv, normal), 1.f);
//	const Vec3 r_out_perp = etai_over_etat * (uv + cos_theta * normal);
//	const Vec3 r_out_parallel = -sqrtf(fabs(1.f - r_out_perp.length_sq())) * normal;
//	return r_out_perp + r_out_parallel;
//}
