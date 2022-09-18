#include "Metal.h"
#include "HitRecord.h"

std::optional<MaterialRecord> Metal::scatter(const Ray& r, const HitRecord& hit_rec)
{
    // Simply reflects rays!
    const Vec3 refl = reflect(r.direction(), hit_rec.normal);

    // Randomize target slightly (unit sphere at the endpoint) for fuzziness!
    const Point3 refl_target = refl + m_fuzz * random_vec_in_unit_sphere();

    const Ray scattered_ray(hit_rec.hit_point, refl_target);

    // On surface hemisphere --> Valid scatter
    if (dot(scattered_ray.direction(), hit_rec.normal) > 0.f)
    {
        return MaterialRecord
        {
            .attenuation = m_albedo,
            .scattered_ray = scattered_ray
        };
    }
    // Under surface hemisphere --> Invalid scatter
    else
    {
        return {};
    }
}
