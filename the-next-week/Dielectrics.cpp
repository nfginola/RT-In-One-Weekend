#include "Dielectrics.h"
#include "HitRecord.h"
#include "Utils.h"

std::optional<MaterialRecord> Dielectrics::scatter(const Ray& r, const HitRecord& hit_rec)
{
    /*
        @todo:
            Read more about the Snells Law solution (refract function)
            Read more about Schlicks Reflectance Approximation
    */


    // If hit front-face --> Entering material --> Air (1) to Material (idx)
    // If hit back-face  --> Exiting material --> Material (idx) to Air (1)
    f32 refraction_ratio = hit_rec.front_face ? (1.f / m_refr_idx) : m_refr_idx;
    
    /*
        sin0' = (n/n') * sin0
        
        If n > n' --> No real solution to Snells Law, equality broken  --> No refraction possible
        For example:
            n = 1.5     // Dense
            n' = 1.0    // Less dense

        Thus, there must be "Total Internal Reflection"
    */
    f32 cos_theta = fmin(dot(-r.direction(), hit_rec.normal), 1.f);
    f32 sin_theta = sqrtf(1.f - cos_theta * cos_theta);
    bool cannot_refract = (refraction_ratio * sin_theta) > 1.0;

    /*
        Real glass has reflectivity which varies by angle.
        Look at a window at a steep angle and it becomes a mirror.

        Cheap Schlick approximation solves this!
    */
    bool schlick_should_reflect = reflectance(cos_theta, refraction_ratio) > random_float();

    Vec3 direction;
    if (cannot_refract || schlick_should_reflect)
        direction = reflect(r.direction(), hit_rec.normal);
    else
        direction = refract(r.direction(), hit_rec.normal, refraction_ratio);

    return MaterialRecord
    {
        .attenuation = Color(1.f),  // Glass surface absorbs nothing
        .scattered_ray = Ray(hit_rec.hit_point, direction)
    };
}

f32 Dielectrics::reflectance(f32 cosine, f32 refl_idx)
{
    // Use Schlick's approximation for reflectance.
    f32 r0 = (1.f - refl_idx) / (1.f + refl_idx);
    r0 = r0 * r0;
    return r0 + (1.f - r0) * powf((1.f - cosine), 5.f);
}
