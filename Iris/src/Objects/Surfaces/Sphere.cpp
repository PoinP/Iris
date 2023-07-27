#include "Sphere.h"

#include <cmath>
#include <Utilities/Utility.h>

Sphere::Sphere(const Point3f& center, float radius, const std::shared_ptr<Material>& material)
    : Surface(material, constructAABB(center, radius))
    , m_Radius(std::max(radius, 0.0f))
{
    m_Position = center;
}

// A sphere is checked through the following equation
// (Px - Cx)^2 + (Py - Cy)^2 + (Pz - Cz)^2 - r^2 = 0
// Where C is the center of the sphere and
// P is our ray which can be represented as
// (O + s * d), where O is the origin, d is the direction
// and s is a scalar which is used to move the direction vector
// through 3D space. If we can find real solutions for s, then 
// our ray has intersected the sphere!
bool Sphere::isHit(const Ray& ray, Context& context, float minT, float maxT) const
{
    float temp;
    if (!m_AABB.isHit(ray, maxT, temp))
        return false;

    const float radiusSquared = m_Radius * m_Radius;
    const Vector3f offset = m_InverseTransformation3 * (ray.origin() - m_Position);
    const Vector3f directrion = m_InverseTransformation3 * ray.direction();

    const float a = dotProduct(directrion, directrion);
    const float b = 2 * dotProduct(directrion, offset);
    const float c = dotProduct(offset, offset) - radiusSquared;

    const float discriminant = b * b - 4 * a * c;

    if (discriminant < 0)
        return false;

    float sqrtDiscriminant = sqrtf(discriminant);
    
    float t = (-b - sqrtDiscriminant) / (2 * a);
    if (t < minT || t > maxT)
    {
        t = (-b + sqrtDiscriminant) / (2 * a);
        if (t < minT || t > maxT)
            return false;
    }

    context.distance = t;
    context.hitPoint = ray.at(t);
    context.material = m_Material.get();
    context.faceNormal = context.hitPoint;

    Vector3f normal = (ray.at(t) - m_Position) / m_Radius;
    context.normal = toUnitVector(transpose(m_InverseTransformation3) * normal);
    context.faceNormal = context.normal;

    const float u = 0.5f + atan2f(-normal.x(), -normal.z()) / 2 * PI;
    const float v = 0.5f - asinf(-normal.y()) / PI;
    context.uv = Point2f(u, v);

    return true;
}

void Sphere::applyTransformations()
{
    m_InverseTransformation3 = invert(m_TransformationMatrix);
}

std::unique_ptr<Surface> Sphere::clone() const
{
    return std::make_unique<Sphere>(*this);
}

AABB Sphere::constructAABB(const Point3f& center, float radius)
{
    return AABB(
        Vector3f(center.x() - radius, center.y() - radius, center.z() - radius),
        Vector3f(center.x() + radius, center.y() + radius, center.z() + radius)
    );
}
