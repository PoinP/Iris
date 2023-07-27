#ifndef REFRACTIVE_H

#define REFRACTIVE_H

#include <Objects/Materials/Reflective.h>

class Refractive : public Material
{
public:
	Refractive(const Color albedo, float refractionIndex, bool smoothShading = true, bool backfaceCulling = false);
	virtual Color shadeScene(const Ray& ray, const Scene& scene, const Surface::Context& context) const override;
	virtual bool hasShadow() const { return false; }

private:
	float m_RefractionIndex;

	float fresnel(float cosTheta_i, float eta) const;
	Color attenuation(const Vector3f& rayDirection, const Vector3f& normal, float distance) const;
	Vector3f calculateBias(const Vector3f& dir, const Vector3f& normal, float bias, bool refracting) const;
};

#endif // !REFRACTIVE_H
