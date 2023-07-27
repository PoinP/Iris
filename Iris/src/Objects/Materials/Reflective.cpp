#include "Reflective.h"

#include <Utilities/Utility.h>
#include <Core/Renderer.h>

Reflective::Reflective(const Color& albedo, float reflectionIndex, bool smoothShading, bool backfaceCulling)
    : Reflective(albedo, reflectionIndex, 0.0f, smoothShading, backfaceCulling)
{
}

Reflective::Reflective(const Color& albedo, float reflectionIndex, float roughness, bool smoothShading, bool backfaceCulling)
    : Material(albedo, smoothShading, backfaceCulling), m_Roughness(clamp(roughness, 0.0f, 1.0f)), m_ReflectionIndex(clamp(reflectionIndex, 0.0f, 1.0f))
{
}

Color Reflective::shadeScene(const Ray& ray, const Scene& scene, const Surface::Context& context) const
{
    if (!scene.settings().flags.reflections)
        return m_Albedo;

    const Vector3f& normal = m_SmoothShading ? context.normal : context.faceNormal;
    const Vector3f& hitPoint = m_SmoothShading ? context.smoothHitPoint : context.hitPoint;
    const Vector3f& rayDir = ray.direction();

    const float biasFactor = 0.1f;
    const Vector3f bias = normal * biasFactor;
    const Vector3f reflectionDir = toUnitVector(vecReflect(rayDir, normal) + m_Roughness * randomVector(-0.5f, 0.5f));
    const Ray reflectionRay(hitPoint + bias, reflectionDir, ray.depth() + 1);
    Color reflectedColor = Renderer::traceRay(reflectionRay, scene);

    return m_Albedo * m_ReflectionIndex * reflectedColor;
}
