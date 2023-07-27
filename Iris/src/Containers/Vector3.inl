#include <stdexcept>
#include <cassert>
#include <cmath>

template<typename Type>
inline Vector3<Type>::Vector3()
	: m_Coords{ Type{}, Type{}, Type{} }
{
}

template<typename Type>
inline Vector3<Type>::Vector3(const Vector2<Type>& vec2, const Type& z)
	: m_Coords {vec2.x(), vec2.y(), z}
{
}

template<typename Type>
inline Vector3<Type>::Vector3(const Type& x, const Vector2<Type>& vec2)
	: m_Coords { x, vec2.x(), vec2.y() }
{
}

template<typename Type>
inline Vector3<Type>::Vector3(const Type& x, const Type& y, const Type& z)
	: m_Coords { x, y, z }
{
}

template<typename Type>
template<typename U>
inline Vector3<Type>::Vector3(const Vector3<U>& other)
	: Vector3((Type)other.x(), (Type)other.y(), (Type)other.z())
{
}


template<typename Type>
inline Vector3<Type> Vector3<Type>::operator-() const
{
	return Vector3(-m_Coords[0], -m_Coords[1], -m_Coords[2]);
}

template<typename Type>
inline const Type& Vector3<Type>::operator[](size_t idx) const
{
	assert(idx >= 0 && idx <= 2);
	return m_Coords[idx];
}

template<typename Type>
inline Type& Vector3<Type>::operator[](size_t idx)
{
	assert(idx >= 0 && idx <= 2);
	return m_Coords[idx];
}

template<typename Type>
inline Vector3<Type>& Vector3<Type>::operator+=(const Vector3& other)
{
	m_Coords[0] += other.m_Coords[0];
	m_Coords[1] += other.m_Coords[1];
	m_Coords[2] += other.m_Coords[2];

	return *this;
}

template<typename Type>
inline Vector3<Type>& Vector3<Type>::operator-=(const Vector3& other)
{
	*this += -other;
	return *this;
}

template<typename Type>
inline Vector3<Type>& Vector3<Type>::operator*=(Type scalar)
{
	m_Coords[0] *= scalar;
	m_Coords[1] *= scalar;
	m_Coords[2] *= scalar;

	return *this;
}

template<typename Type>
inline Vector3<Type>& Vector3<Type>::operator/=(Type scalar)
{
	*this *= (1 / scalar);
	return *this;
}

template<typename Type>
inline const Type& Vector3<Type>::x() const
{
	return m_Coords[0];
}

template<typename Type>
inline const Type& Vector3<Type>::y() const
{
	return m_Coords[1];
}

template<typename Type>
inline const Type& Vector3<Type>::z() const
{
	return m_Coords[2];
}

template<typename Type>
inline Type& Vector3<Type>::x()
{
	return m_Coords[0];
}

template<typename Type>
inline Type& Vector3<Type>::y()
{
	return m_Coords[1];
}

template<typename Type>
inline Type& Vector3<Type>::z()
{
	return m_Coords[2];
}

template<typename Type>
inline void Vector3<Type>::normalize()
{
	*this = toUnitVector(*this);
}

template<typename Type>
inline Type Vector3<Type>::length() const
{
	return sqrt(lengthSquared());
}

template<typename Type>
inline Type Vector3<Type>::lengthSquared() const
{
	return dotProduct(*this, *this);
}

template<typename Type>
inline Vector3<Type> operator+(Vector3<Type> lhs, const Vector3<Type>& rhs)
{
	return lhs += rhs;
}

template<typename Type>
inline Vector3<Type> operator-(Vector3<Type> lhs, const Vector3<Type>& rhs)
{
	return lhs -= rhs;
}

template<typename Type>
inline Vector3<Type> operator*(Vector3<Type> lhs, Type scalar)
{
	return lhs *= scalar;
}

template<typename Type>
inline Vector3<Type> operator*(Type scalar, Vector3<Type> rhs)
{
	return rhs *= scalar;
}

template<typename Type>
inline Vector3<Type> operator/(Vector3<Type> lhs, Type scalar)
{
	return lhs /= scalar;
}

template<typename Type>
inline bool operator==(const Vector3<Type>& lhs, const Vector3<Type>& rhs)
{
	return lhs.x() == rhs.x() &&
		   lhs.y() == rhs.y() &&
		   lhs.z() == rhs.z();
}

template<typename Type>
inline bool operator!=(const Vector3<Type>& lhs, const Vector3<Type>& rhs)
{
	return !(lhs == rhs);
}

template<typename Type>
inline Type dotProduct(const Vector3<Type>& lhs, const Vector3<Type>& rhs)
{
	return lhs.x() * rhs.x() +
		   lhs.y() * rhs.y() +
		   lhs.z() * rhs.z();
}

template<typename Type>
inline Vector3<Type> crossProduct(const Vector3<Type>& lhs, const Vector3<Type>& rhs)
{
	return Vector3<Type>(lhs.y() * rhs.z() - lhs.z() * rhs.y(),
						 lhs.z() * rhs.x() - lhs.x() * rhs.z(),
						 lhs.x() * rhs.y() - lhs.y() * rhs.x());
}

template<typename Type>
inline Vector3<Type> toUnitVector(const Vector3<Type>& vec)
{
	const Type length = vec.length();
	assert(length > 0);
	return vec * (1 / length);
}

inline Vector3f vecReflect(const Vector3f& dir, const Vector3f& normal)
{
	return dir - 2 * (dotProduct(dir, normal)) * normal;
}

inline Vector3f vecRefract(const Vector3f& dir, const Vector3f& normal, float cos, float eta)
{
	const float kValue = 1.0f - eta * eta * (1.0f - cos * cos);

	if (kValue < 0)
		return Vector3f();

	return dir * eta + normal * (cos * eta - sqrtf(kValue));
}