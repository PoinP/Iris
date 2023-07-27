#ifndef DIFFUSE_H

#define DIFFUSE_H

#include <Objects/Material.h>

class Diffuse : public Material
{
public:
	Diffuse(Color albedo, bool smoothShading = true, bool backfaceCulling = false);
	Diffuse(Color albedo, unsigned lightDepth, bool smoothShading, bool backfaceCulling);
	virtual Color shadeScene(const Ray& ray, const Scene& scene, const Surface::Context& context) const override;

private:
	unsigned m_LightDepth;

	Vector3f diffuseReflect() const;
	Color calculateDirectLight(const Ray& ray, const Scene& scene, const Surface::Context& context) const;
	Color calculateIndirectLight(const Ray& ray, const Scene& scene, const Surface::Context& context) const;
	Color calculateIndirectLightAlt(const Ray& ray, const Scene& scene, const Surface::Context& context) const; // Alternative algorithm for indirect light
	Matrix3 constructLocalCoordinateSystem(const Point3f& hitPoint, const Vector3f& rayDirection, const Vector3f& normal) const;
};

#endif // !DIFFUSE_H
