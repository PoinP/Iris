#ifndef RAY_H

#define RAY_H

#include <Containers/Vector3.h>

class Ray
{
public:
	Ray(const Point3f& origin, const Point3f& direction, unsigned depth = 0);

	const Point3f& origin() const;
	const Vector3f& direction() const;
	Point3f at(float t) const;
	unsigned depth() const;
private:
	Point3f m_Origin;
	Point3f m_Direction;
	unsigned m_Depth;
};

#endif // !RAY_H