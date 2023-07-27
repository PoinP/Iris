#ifndef REFLECTIVE_H

#define REFLECTIVE_H

#include <Objects/Material.h>

class Reflective : public Material
{
public:
	Reflective(const Color& albedo, float reflectionIndex, bool smoothShading = true, bool backfaceCulling = false);
	Reflective(const Color& albedo, float reflectionIndex, float roughness, bool smoothShading = true, bool backfaceCulling = false);
	virtual Color shadeScene(const Ray& ray, const Scene& scene, const Surface::Context& context) const override;

private:
	float m_Roughness;
	float m_ReflectionIndex;
};


#endif // !REFLECTIVE_H
