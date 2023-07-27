#include "Ray.h"

Ray::Ray(const Point3f& origin, const Point3f& direction, unsigned depth)
	: m_Origin(origin), m_Direction(direction), m_Depth(depth)
{
}

const Point3f& Ray::origin() const
{
	return m_Origin;
}

const Vector3f& Ray::direction() const
{
	return m_Direction;
}

unsigned Ray::depth() const
{
	return m_Depth;
}

Point3f Ray::at(float t) const
{
	return m_Origin + t * m_Direction;
}
