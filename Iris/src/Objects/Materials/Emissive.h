#ifndef EMISSIVE_H

#define EMISSIVE_H

#include <Objects/Material.h>

class Emissive : public Material
{
public:
	Emissive(const Color& albedo, float intensity, bool smoothShading = true, bool backfaceCulling = false)
		: Material(albedo, smoothShading, backfaceCulling), m_Intensity(std::max(intensity, 0.0f)) {}

	virtual Color shadeScene(const Ray& ray, const Scene& scene, const Surface::Context& context) const override { return m_Albedo * m_Intensity; }

private:
	float m_Intensity;
};

#endif // !EMISSIVE_H