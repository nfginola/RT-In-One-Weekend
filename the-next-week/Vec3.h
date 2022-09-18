#pragma once
#include "Common.h"
#include <cmath>
#include "Utils.h"

struct Vec3
{ 
	f32 x{ 0.0 };
	f32 y{ 0.0 };
	f32 z{ 0.0 };

	Vec3(f32 value) : x(value), y(value), z(value) {}
	Vec3() : x(0.0), y(0.0), z(0.0) {}
	Vec3(f32 x, f32 y, f32 z) : x(x), y(y), z(z) {}

	inline Vec3 operator-() const { return Vec3(-x, -y, -z); }
	inline Vec3& operator+=(const Vec3& other) { x += other.x; y += other.y; z += other.z; return *this; }
	inline Vec3& operator-=(const Vec3& other) { x -= other.x; y -= other.y; z -= other.z; return *this; }
	inline Vec3& operator*=(f32 t) { x *= t; y *= t; z *= t; return *this; }
	inline Vec3& operator/=(f32 t) { x /= t; y /= t; z /= t; return *this; }
	inline f32 length_sq() const { return x * x + y * y + z * z; }
	inline f32 length() const { return sqrt(length_sq()); }
	inline Vec3& normalized() { *this /= length(); return *this; }

	inline bool near_zero() const
	{
		constexpr f32 eps = std::numeric_limits<f32>::epsilon();
		return (x < eps) && (y < eps) && (z < eps);
	}

};

using Point3 = Vec3;
using Color = Vec3;

Vec3 operator+(const Vec3& lh, const Vec3& rh);
Vec3 operator-(const Vec3& lh, const Vec3& rh);
Vec3 operator*(const Vec3& lh, f32 t);
Vec3 operator*(f32 t, const Vec3& rh);
Vec3 operator*(const Vec3& lh, const Vec3& rh);
Vec3 operator/(const Vec3& lh, f32 t);
Vec3 operator/(f32 t, const Vec3& rh);

/*
	Inlined
*/
//f32 dot(const Vec3& lh, const Vec3& rh);
//Vec3 cross(const Vec3& lh, const Vec3& rh);
//Vec3 random_vec(f32 min, f32 max);

/*
	Bug fixed:
		Mixed incident/normal arguments..
*/
//Vec3 reflect(const Vec3& incident, const Vec3& normal);

// Rejection method
//Vec3 random_vec_in_unit_sphere();

// Get unit vector to sphere surface!
//Vec3 random_unit_vec();

// Uniform scatter direction for all angles away from the hit point
//Vec3 random_in_hemisphere(const Vec3& normal);

//Vec3 refract(const Vec3& uv, const Vec3& normal, f32 etai_over_etat);

//Vec3 random_in_unit_disk();


inline Vec3 random_in_unit_disk()
{
	while (true)
	{
		const Vec3 p = Vec3(random_float(-1.f, 1.f), random_float(-1.f, 1.f), 0.f);
		if (p.length_sq() >= 1.f)
			continue;
		return p;
	}
}





inline f32 dot(const Vec3& lh, const Vec3& rh)
{
	return lh.x * rh.x + lh.y * rh.y + lh.z * rh.z;
}

inline Vec3 cross(const Vec3& lh, const Vec3& rh)
{
	return Vec3(
		lh.y * rh.z - lh.z * rh.y,
		lh.z * rh.x - lh.x * rh.z,
		lh.x * rh.y - lh.y * rh.x);
}

inline Vec3 random_vec(f32 min, f32 max)
{
	return Vec3((f32)random_float(min, max), (f32)random_float(min, max), (f32)random_float(min, max));
}

inline Vec3 reflect(const Vec3& incident, const Vec3& normal)
{
	return incident + 2.f * dot(-incident, normal) * normal;
}

inline Vec3 random_vec_in_unit_sphere()
{
	while (true)
	{
		const Vec3 p = random_vec(-1.f, 1.f);
		if (p.length_sq() >= 1.f)
			continue;
		return p;
	}
}

inline Vec3 random_unit_vec()
{
	return random_vec_in_unit_sphere().normalized();
}

inline Vec3 random_in_hemisphere(const Vec3& normal)
{
	Vec3 in_unit_sphere = random_vec_in_unit_sphere();
	if (dot(in_unit_sphere, normal) > 0.0) // In the same hemisphere as the normal
		return in_unit_sphere;
	else
		return -in_unit_sphere;
}

//Vec3 random_in_unit_disk()
//{
//	while (true)
//	{
//		const Vec3 p = Vec3(random_float(-1.f, 1.f), random_float(-1.f, 1.f), 0.f);
//		if (p.length_sq() >= 1.f)
//			continue;
//		return p;
//	}
//}

inline Vec3 refract(const Vec3& uv, const Vec3& normal, f32 etai_over_etat)
{
	const f32 cos_theta = fmin(dot(-uv, normal), 1.f);
	const Vec3 r_out_perp = etai_over_etat * (uv + cos_theta * normal);
	const Vec3 r_out_parallel = -sqrtf(fabs(1.f - r_out_perp.length_sq())) * normal;
	return r_out_perp + r_out_parallel;
}

