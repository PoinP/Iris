#ifndef VECTOR_4_H

#define VECTOR_4_H

#include "Vector2.h"
#include "Vector3.h"

template<typename Type>
class Vector4
{
public:
	Vector4();
	Vector4(const Vector2<Type>& vec2, const Type& z, const Type& w);
	Vector4(const Type& x, const Type& y, const Vector2<Type>& vec2);
	Vector4(const Vector3<Type>& vec3, const Type& w = 1);
	Vector4(const Type& x, const Vector3<Type>& vec3);
	Vector4(const Type& x, const Type& y, const Type& z, const Type& w);

	template<typename U>
	explicit Vector4(const Vector4<U>& other);

	explicit operator Vector3f() const;

	Vector4 operator-() const;

	const Type& operator[](size_t idx) const;
	Type& operator[](size_t idx);

	Vector4& operator+= (const Vector4& other);
	Vector4& operator-= (const Vector4& other);
	Vector4& operator*= (Type scalar);
	Vector4& operator/= (Type scalar);

	const Type& x() const;
	const Type& y() const;
	const Type& z() const;
	const Type& w() const;

	Type& x();
	Type& y();
	Type& z();
	Type& w();

	void normalize();

	Type length() const;
	Type lengthSquared() const;
private:
	float m_Coords[4];
};

using Vector4f = Vector4<float>;
using Vector4i = Vector4<int>;

using Point4f = Vector4<float>;
using Point4i = Vector4<int>;

template<typename Type>
Vector4<Type> operator+ (Vector4<Type> lhs, const Vector4<Type>& rhs);

template<typename Type>
Vector4<Type> operator-(Vector4<Type> lhs, const Vector4<Type>& rhs);

template<typename Type>
Vector4<Type> operator*(Vector4<Type> lhs, Type scalar);
template<typename Type>
Vector4<Type> operator*(Type scalar, Vector4<Type> rhs);
template<typename Type>
Vector4<Type> operator/(Vector4<Type> lhs, Type scalar);

template<typename Type>
bool operator==(const Vector4<Type>& lhs, const Vector4<Type>& rhs);
template<typename Type>
bool operator!=(const Vector4<Type>& lhs, const Vector4<Type>& rhs);

template<typename Type>
Type dotProduct(const Vector4<Type>& lhs, const Vector4<Type>& rhs);
template<typename Type>
Vector4<Type> toUnitVector(const Vector4<Type>& vec);

#include "Vector4.inl"

#endif // !VECTOR_4_H