#include "Diffuse.h"

#include <Core/Renderer.h>
#include <Utilities/Utility.h>
#include <Containers/Matrix3.h>

Diffuse::Diffuse(Color albedo, bool smoothShading, bool backfaceCulling)
    : Diffuse(albedo, 5, smoothShading, backfaceCulling)
{
}

Diffuse::Diffuse(Color albedo, unsigned lightDepth, bool smoothShading, bool backfaceCulling)
	: Material(albedo, smoothShading, backfaceCulling), m_LightDepth(lightDepth)
{
}

Color Diffuse::shadeScene(const Ray& ray, const Scene& scene, const Surface::Context& context) const
{
	if (!scene.settings().flags.globalIllumination)
		return calculateDirectLight(ray, scene, context);
	
	return (calculateDirectLight(ray, scene, context) + calculateIndirectLight(ray, scene, context)) / 2.0f;
}

Vector3f Diffuse::diffuseReflect() const
{
	float randomAngle = PI * randomFloat(0, 1.0f);
	Vector3f rayDirection(cos(randomAngle), sin(randomAngle), 0.0f);

	randomAngle = 2 * PI * randomFloat(0, 1.0f);
	const float cosTheta = cos(randomAngle);
	const float sinTheta = sin(randomAngle);

	Matrix3 rotationMatrix(
		Vector3f(cosTheta, 0.0f, -sinTheta),
		Vector3f(0.0f, 1.0f, 0.0f),
		Vector3f(sinTheta, 0.0f, cosTheta)
	);

	return rotationMatrix * rayDirection;
}

Color Diffuse::calculateDirectLight(const Ray& ray, const Scene& scene, const Surface::Context& context) const
{
	const std::vector<Light>& lights = scene.lights();

	Color accumulatedColor;
	for (const Light& light : lights)
	{
		const Point3f& hitPoint = m_SmoothShading ? context.smoothHitPoint : context.hitPoint;
		const Vector3f& normal = m_SmoothShading ? context.normal : context.faceNormal;

		Vector3f lightDir = light.position() - hitPoint;
		const float sphereRadius = lightDir.length();

		lightDir.normalize();

		// Lambert's cosine law
		const float angleOffset = dotProduct(lightDir, normal);

		if (angleOffset <= 0.0f) continue;

		bool isInShadow = false;
		const float biasFactor = m_SmoothShading ? 0.01f : 0.1f;
		const Vector3f bias = normal * biasFactor;
		const Ray shadowRay(hitPoint + bias, lightDir);

		Surface::Context tempContext;
		const float closestObject = sphereRadius;
		for (const std::unique_ptr<Surface>& object : scene.objects())
		{
			if (object->isHit(shadowRay, tempContext, 0.0001f, closestObject))
			{
				if (!tempContext.material->hasShadow())
					continue;

				isInShadow = true;
				break;
			}
		}

		if (isInShadow) continue;

		const float sphereArea = 4.0f * PI * sphereRadius * sphereRadius;
		const float& reduction = light.intensity() / sphereArea * angleOffset;

		accumulatedColor += reduction * m_Albedo;
	}

	return accumulatedColor;
}

Color Diffuse::calculateIndirectLight(const Ray& ray, const Scene& scene, const Surface::Context& context) const
{
	if (ray.depth() >= m_LightDepth)
		return Color();

	Vector3f rayDirection = diffuseReflect();
	const Matrix3 localCoordinateSystem = constructLocalCoordinateSystem(context.hitPoint, ray.direction(), context.faceNormal);
	rayDirection = localCoordinateSystem * rayDirection;
	const Point3f origin = context.hitPoint + context.faceNormal * 0.001f;

	Ray reflectedRay(origin, rayDirection, ray.depth() + 1);

	return m_Albedo * Renderer::traceRay(reflectedRay, scene);
}

Color Diffuse::calculateIndirectLightAlt(const Ray& ray, const Scene& scene, const Surface::Context& context) const
{
	if (ray.depth() >= m_LightDepth)
		return Color();

	Vector3f direction = toUnitVector(context.faceNormal + randomInHemiSphere(context.faceNormal));
	Point3f origin = context.hitPoint + context.faceNormal * 0.001f;

	Ray reflRay(origin, direction, ray.depth() + 1);
	return Renderer::traceRay(reflRay, scene);
}

Matrix3 Diffuse::constructLocalCoordinateSystem(const Point3f& hitPoint, const Vector3f& rayDirection, const Vector3f& normal) const
{
	// A bias used in order to avoid situations in which
	// the cross product of the ray's direction and the
	// triangle's normal are parallel to each other
	const Vector3f bias(0.000001f, 0.000001f, 0.000001f);

	const Vector3f up      = normal;
	const Vector3f right   = toUnitVector(crossProduct(rayDirection + bias, up));
	const Vector3f forward = toUnitVector(crossProduct(up, right));
	
	return Matrix3(right, up, forward);
}
