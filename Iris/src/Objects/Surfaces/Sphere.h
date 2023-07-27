#ifndef SPHERE_H

#define SPHERE_H

#include <Objects/Surface.h>
#include <Objects/Material.h>
#include <Containers/Vector3.h>

class Sphere : public Surface
{
public:
	Sphere(const Point3f& center, float radius, const std::shared_ptr<Material>& material);
	virtual bool isHit(const Ray& ray, Context& context, float minT, float maxT) const override;
	virtual void applyTransformations() override;
	virtual std::unique_ptr<Surface> clone() const override;

private:
	float m_Radius;
	Matrix3 m_InverseTransformation3;

	static AABB constructAABB(const Point3f& center, float radius);
};

#endif // !SPHERE_H
