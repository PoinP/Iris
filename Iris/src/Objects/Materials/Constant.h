#ifndef CONSTANT_H

#include <Objects/Material.h>

#define CONSTANT_H

class Constant : public Material
{
public:
	Constant(const Color& albedo, bool smoothShading, bool backfaceCulling) : Material(albedo, smoothShading, backfaceCulling) {}
	virtual Color shadeScene(const Ray& ray, const Scene& scene, const Surface::Context& context) const override { return m_Albedo; }
};

#endif // !CONSTANT_H
