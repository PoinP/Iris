#ifndef CAMERA_H

#define CAMERA_H

#include "Ray.h"
#include "Containers/Vector2.h"
#include <Containers/Vector3.h>
#include <Containers/Matrix4.h>

#include <vector>

class Camera
{
public:
	Camera(Point3f origin, unsigned width, unsigned height, float fov = 60.0f, float focalLength = 1, float apertureSize = 0);

	Ray getRay(float x, float y) const;
	void generateRays(unsigned x, unsigned y, std::vector<Ray>& rays, unsigned sampleCount) const;

	void setFOV(float degrees);
	void setOrigin(const Point3f& origin);
	void setFocalLength(float focalLength);
	void setApertureSize(float apertureSize);
	void setResolution(unsigned width, unsigned height);

	void lookAt(const Point3f& point);
	void transform(const Matrix3& transformationMatrix);

	void pan(float degrees);
	void tilt(float degrees);
	void roll(float degrees);

	void dolly(float offset);
	void boom(float offset);
	void truck(float offset);

	void rotateOriginX(float degrees, const Point3f& orgin);
	void rotateOriginY(float degrees, const Point3f& orgin);
	void rotateOriginZ(float degrees, const Point3f& orgin);

private:
	Point3f m_Origin;
	unsigned m_Width;
	unsigned m_Height;
	float m_FOV;
	float m_AspectRatio;
	float m_FocalLength;
	float m_ApertureSize;

	Matrix3 m_View;
	Matrix3 m_InverseView;

	void toNDC(Vector2f& pixel) const;
	void toScreenSpace(Vector2f& ndc) const;
};

#endif // !CAMERA_H