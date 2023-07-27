#ifndef MATERIAL_H

#define MATERIAL_H

#include <Containers/Color.h>
#include <Objects/Surface.h>
#include <Core/Scene.h>
#include <Core/Ray.h>

class Material
{
public:
	Material(const Color& albedo, bool smoothShading = true, bool backfaceCulling = false) 
		: m_Albedo(albedo), m_SmoothShading(smoothShading), m_BackfaceCulling(backfaceCulling) {}
	virtual ~Material() {}

	virtual Color shadeScene(const Ray& ray, const Scene& scene, const Surface::Context& context) const = 0;
	virtual bool hasShadow() const { return true; }
	bool hasBackfaceCulling() const { return m_BackfaceCulling; }

protected:
	Color m_Albedo;
	bool m_SmoothShading;
	bool m_BackfaceCulling;

};

#endif // !MATERIAL_H
