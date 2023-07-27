#ifndef SURFACE_H

#define SURFACE_H

#include <Core/Ray.h>
#include <Core/AABB.h>
#include <Containers/Color.h>
#include <Containers/Matrix4.h>

class Material;

class Surface
{
public:
	struct Context
	{
		Point2f uv{};				// UV coordinates
		float distance{};			// The distance from the ray origin
		Vector3f normal{};			// Interploated normal
		Vector3f hitPoint{};		// Vertex hit point
		Vector3f faceNormal{};		// Face normal
		Vector3f smoothHitPoint{};	// Interpolated hit point
		const Material* material{}; // Material
	};

public:
	virtual ~Surface() {}
	virtual bool isHit(const Ray& ray, Context& context, float minT, float maxT) const = 0;
	virtual void applyTransformations() = 0;
	virtual std::unique_ptr<Surface> clone() const = 0;

	Point3f position() const    { return m_Position; }
	const AABB& getAABB() const { return m_AABB; }

	void setMaterial(std::shared_ptr<Material> material) { m_Material = material; }

	void rotateX(float degrees);
	void rotateY(float degrees);
	void rotateZ(float degrees);

	void scale(float scale);
	void scaleX(float scale);
	void scaleY(float scale);
	void scaleZ(float scale);

	void translate(Vector3f pos);
	void translateX(float pos);
	void translateY(float pos);
	void translateZ(float pos);

protected:
	AABB m_AABB;
	Point3f m_Position;
	Matrix3 m_TransformationMatrix;
	std::shared_ptr<Material> m_Material;

	Surface(std::shared_ptr<Material> material, const AABB& aabb) 
		: m_Material(material), m_AABB(aabb) {}
};

#endif // !SURFACE_H