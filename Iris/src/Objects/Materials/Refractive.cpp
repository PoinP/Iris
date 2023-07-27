#include "Refractive.h"

#include <Utilities/Utility.h>
#include <Core/Renderer.h>

Refractive::Refractive(const Color albedo, float refractionIndex, bool smoothShading, bool backfaceCulling)
    : Material(albedo, smoothShading, backfaceCulling), m_RefractionIndex(refractionIndex)
{
}

Color Refractive::shadeScene(const Ray& ray, const Scene& scene, const Surface::Context& context) const
{
    const Vector3f& rayDir = ray.direction();
    const Point3f& hitPoint = m_SmoothShading ? context.smoothHitPoint : context.hitPoint;
    const Vector3f& surfaceNormal = m_SmoothShading ? context.normal : context.faceNormal;

    Vector3f refrNormal = surfaceNormal;
    float cosTheta_i = dotProduct(rayDir, refrNormal);

    // Used for validating the Phong interpolation normal.
    // Sometimes the normal points in the back direction and
    // artefacts appear. It is especially visible on grazing angles.
    float faceCosTheta = dotProduct(rayDir, context.faceNormal);

    if (cosTheta_i > 0.0f && faceCosTheta < 0.0f)
    {
        refrNormal = context.faceNormal;
        cosTheta_i = dotProduct(rayDir, refrNormal);
    }

    float etai = 1.0f;
    float etat = m_RefractionIndex;

    if (cosTheta_i < 0)
    {
        cosTheta_i = -cosTheta_i;
    }
    else
    {
        refrNormal = -refrNormal;
        std::swap(etai, etat);
    }

    const float eta = etai / etat;
    const float reflectance = fresnel(cosTheta_i, eta);

    Color refractedColor, reflectedColor;
    const Color attenuatedColor = attenuation(rayDir, context.faceNormal, context.distance); // < Beer's law

    if (reflectance < 1.0f)
    {
        Vector3f bias = calculateBias(rayDir, context.faceNormal, 0.001f, true);
        Vector3f refraction = vecRefract(rayDir, refrNormal, cosTheta_i, eta);
        Ray refractedRay(context.hitPoint + bias, refraction, ray.depth() + 1);

        refractedColor = scene.settings().flags.refractions ? 
            Renderer::traceRay(refractedRay, scene) : m_Albedo;
    }

    const float biasFactor = 0.01f;
    Vector3f bias = calculateBias(rayDir, context.faceNormal, biasFactor, false);
    Ray reflectedRay(hitPoint + bias, vecReflect(rayDir, refrNormal), ray.depth() + 1);
    reflectedColor = scene.settings().flags.reflections ? 
        Renderer::traceRay(reflectedRay, scene) : m_Albedo;

    return attenuatedColor * (reflectedColor * reflectance + refractedColor * (1 - reflectance));
}

float Refractive::fresnel(float cosTheta_i, float eta) const
{
    float sinTheta_t = eta * std::max(0.0f, sqrtf(1.0f - square(cosTheta_i)));

    if (sinTheta_t >= 1.0f)
        return 1.0f;

    float cosTheta_t = std::max(0.0f, sqrtf(1.0f - square(sinTheta_t)));

    const float rParal = (eta * cosTheta_i - cosTheta_t) /
                         (eta * cosTheta_i + cosTheta_t);

    const float rPerp = (cosTheta_i - eta * cosTheta_t) /
                        (cosTheta_i + eta * cosTheta_t);

    return (square(rParal) + square(rPerp)) * 0.5f;
}

Color Refractive::attenuation(const Vector3f& rayDirection, const Vector3f& normal, float distance) const
{
    if (dotProduct(rayDirection, normal) < 0)
        return Color(1.0f, 1.0f, 1.0f);

    return Color(expf((m_Albedo.r() - 1.0f) * distance),
                 expf((m_Albedo.g() - 1.0f) * distance),
                 expf((m_Albedo.b() - 1.0f) * distance));
}

Vector3f Refractive::calculateBias(const Vector3f& dir, const Vector3f& normal, float bias, bool refracting) const
{
    const bool isOutside = dotProduct(dir, normal) < 0.0f;
    Vector3f nBias = normal * bias;
    nBias = isOutside ? nBias : -nBias;
    return refracting ? -nBias : nBias;
}