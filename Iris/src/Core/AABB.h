#ifndef AABB_H

#define AABB_H

#include <Core/Ray.h>
#include <Containers/Vector3.h>

struct BoxPair;

class AABB
{
public:
	AABB(const Vector3f& min, const Vector3f& max);
	bool isHit(const Ray& ray, float maxT, float& currMaxT) const;

	Vector3f& min();
	Vector3f& max();
	const Vector3f& min() const;
	const Vector3f& max() const;

	void combine(const AABB& other);
	bool intersects(const AABB& other) const;
	BoxPair split(unsigned axis) const;

private:
	Vector3f m_Min;
	Vector3f m_Max;
};

struct BoxPair
{
	AABB leftBox;
	AABB rightBox;
};


#endif // !AABB_H
