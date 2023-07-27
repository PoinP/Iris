#ifndef VECTOR_3_H

#define VECTOR_3_H

#include <Containers/Vector2.h>

template<typename Type>
class Vector3
{
public:
	Vector3();
	Vector3(const Vector2<Type>& vec2, const Type& z);
	Vector3(const Type& x, const Vector2<Type>& vec2);
	Vector3(const Type& x, const Type& y, const Type& z);

	template<typename U>
	explicit Vector3(const Vector3<U>& other);

	Vector3 operator-() const;

	const Type& operator[](size_t idx) const;
	Type& operator[](size_t idx);

	Vector3& operator+= (const Vector3& other);
	Vector3& operator-= (const Vector3& other);
	Vector3& operator*= (Type scalar);
	Vector3& operator/= (Type scalar);

	const Type& x() const;
	const Type& y() const;
	const Type& z() const;

	Type& x();
	Type& y();
	Type& z();

	void normalize();

	Type length() const;
	Type lengthSquared() const;
private:
	Type m_Coords[3];
};

using Vector3f = Vector3<float>;
using Vector3i = Vector3<int>;

using Point3f = Vector3<float>;
using Point3i = Vector3<int>;

template<typename Type>
Vector3<Type> operator+ (Vector3<Type> lhs, const Vector3<Type>& rhs);

template<typename Type>
Vector3<Type> operator-(Vector3<Type> lhs, const Vector3<Type>& rhs);

template<typename Type>
Vector3<Type> operator*(Vector3<Type> lhs, Type scalar);
template<typename Type>
Vector3<Type> operator*(Type scalar, Vector3<Type> rhs);
template<typename Type>
Vector3<Type> operator/(Vector3<Type> lhs, Type scalar);

template<typename Type>
bool operator==(const Vector3<Type>& lhs, const Vector3<Type>& rhs);
template<typename Type>
bool operator!=(const Vector3<Type>& lhs, const Vector3<Type>& rhs);

template<typename Type>
Type dotProduct(const Vector3<Type>& lhs, const Vector3<Type>& rhs);
template<typename Type>
Vector3<Type> crossProduct(const Vector3<Type>& lhs, const Vector3<Type>& rhs);
template<typename Type>
Vector3<Type> toUnitVector(const Vector3<Type>& vec);

Vector3f vecReflect(const Vector3f& dir, const Vector3f& normal);
Vector3f vecRefract(const Vector3f& dir, const Vector3f& normal, float cos, float eta);

#include "Vector3.inl"

#endif // !VECTOR_3_H