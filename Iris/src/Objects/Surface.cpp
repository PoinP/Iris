#include "Surface.h"

#include <Utilities/Utility.h>

void Surface::rotateX(float degrees)
{
	const float rad = fromDegreesToRadians(degrees);

	const float cosTheta = cos(rad);
	const float sinTheta = sin(rad);

	const Matrix3 rotationMatrix(
		Vector3f(1.0f, 0.0f, 0.0f),
		Vector3f(0.0f, cosTheta, sinTheta),
		Vector3f(0.0f, -sinTheta, cosTheta)
	);

	m_TransformationMatrix *= rotationMatrix;
}

void Surface::rotateY(float degrees)
{
	const float rad = fromDegreesToRadians(degrees);

	const float cosTheta = cos(rad);
	const float sinTheta = sin(rad);

	const Matrix3 rotationMatrix(
		Vector3f(cosTheta, 0.0f, -sinTheta),
		Vector3f(0.0f, 1.0f, 0.0f),
		Vector3f(sinTheta, 0.0f, cosTheta)
	);

	m_TransformationMatrix *= rotationMatrix;
}

void Surface::rotateZ(float degrees)
{
	const float rad = fromDegreesToRadians(degrees);

	const float cosTheta = cos(rad);
	const float sinTheta = sin(rad);

	const Matrix3 rotationMatrix(
		Vector3f(cosTheta, sinTheta, 0.0f),
		Vector3f(-sinTheta, cosTheta, 0.0f),
		Vector3f(0.0f, 0.0f, 1.0f)
	);

	m_TransformationMatrix *= rotationMatrix;
}

void Surface::scale(float scale)
{
	scaleX(scale);
	scaleY(scale);
	scaleZ(scale);
}

void Surface::scaleX(float scale)
{
	m_TransformationMatrix.col0() *= scale;
}

void Surface::scaleY(float scale)
{
	m_TransformationMatrix.col1() *= scale;
}

void Surface::scaleZ(float scale)
{
	m_TransformationMatrix.col2() *= scale;
}

void Surface::translate(Vector3f pos)
{
	translateX(pos.x());
	translateY(pos.y());
	translateZ(pos.z());
}

void Surface::translateX(float pos)
{
	m_Position.x() += pos;
}

void Surface::translateY(float pos)
{
	m_Position.y() += pos;
}

void Surface::translateZ(float pos)
{
	m_Position.z() += pos;
}
