#include "AABB.h"

AABB::AABB(const Vector3f& min, const Vector3f& max)
    : m_Min(min), m_Max(max)
{
}

bool AABB::isHit(const Ray& ray, float maxT, float& currMaxT) const
{
    float t0 = std::numeric_limits<float>::min();
    float t1 = std::numeric_limits<float>::max();

    for (int axis = 0; axis < 3; axis++) 
    {
        float currMin, currMax;
        if (ray.direction()[axis] >= 0) 
        {
            currMax = (m_Max[axis] - ray.origin()[axis]) / ray.direction()[axis];
            if (currMax < 0.0f)
                return false;

            currMin = (m_Min[axis] - ray.origin()[axis]) / ray.direction()[axis];
        }
        else
        {
            currMax = (m_Min[axis] - ray.origin()[axis]) / ray.direction()[axis];
            if (currMax < 0.0f)
                return false;

            currMin = (m_Max[axis] - ray.origin()[axis]) / ray.direction()[axis];
        }

        if (t0 > currMax || t1 < currMin)
            return false;

        t0 = std::max(t0, currMin);
        t1 = std::min(t1, currMax);
    }

    if (t1 > maxT)
        return false;

    constexpr float bias = 0.001f;
    currMaxT = t1 + bias;

    return true;
}

Vector3f& AABB::min()
{
    return m_Min;
}

Vector3f& AABB::max()
{
    return m_Max;
}

const Vector3f& AABB::min() const
{
    return m_Min;
}

const Vector3f& AABB::max() const
{
    return m_Max;
}

void AABB::combine(const AABB& other)
{
    for (int axis = 0; axis < 3; axis++)
    {
        m_Min[axis] = std::min(other.min()[axis], m_Min[axis]);
        m_Max[axis] = std::max(other.max()[axis], m_Max[axis]);
    }
}

bool AABB::intersects(const AABB& other) const
{
    for (int axis = 0; axis < 3; axis++)
    {
        if (m_Min[axis] > other.max()[axis] ||
            m_Max[axis] < other.min()[axis])
            return false;
    }

    return true;
}

BoxPair AABB::split(unsigned axis) const
{
    assert(axis >= 0 && axis <= 2);

    const float middle = (m_Max[axis] + m_Min[axis]) / 2.0f;
    Vector3f maxBound = m_Max;
    maxBound[axis] = middle;

    Vector3f minBound = m_Min;
    minBound[axis] = middle;

    return { AABB(m_Min, maxBound), AABB(minBound, m_Max) };
}
