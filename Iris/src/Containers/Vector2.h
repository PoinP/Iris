#ifndef VECTOR_2_H

#define VECTOR_2_H

template<typename Type>
class Vector2
{
public:
	Vector2();
	Vector2(Type x, Type y);

	template<typename U>
	explicit Vector2(const Vector2<U>& other);

	const Type& x() const;
	const Type& y() const;

	Type& x();
	Type& y();

	Vector2 operator-() const;

	Vector2& operator+=(Vector2 p);
	Vector2& operator-=(Vector2 p);
	Vector2& operator*=(Type scalar);
	Vector2& operator/=(Type scalar);

private:
	float m_X;
	float m_Y;
};

using Vector2f = Vector2<float>;
using Vector2i = Vector2<int>;

using Point2f = Vector2<float>;
using Point2i = Vector2<int>;

template<typename Type>
Vector2<Type> operator+(Vector2<Type> p1, Vector2<Type> p2);
template<typename Type>
Vector2<Type> operator-(Vector2<Type> p1, Vector2<Type> p2);
template<typename Type>
Vector2<Type> operator*(Vector2<Type> p, Type scalar);
template<typename Type>
Vector2<Type> operator*(Type scalar, Vector2<Type> p);
template<typename Type>
Vector2<Type> operator/(Vector2<Type> p, Type scalar);

template<typename Type>
bool operator==(Vector2<Type> p1, Vector2<Type> p2);
template<typename Type>
bool operator!=(Vector2<Type> p1, Vector2<Type> p2);

template<typename Type>
float dotProduct(Vector2<Type> p1, Vector2<Type> p2);

#include "Vector2.inl"

#endif // !VECOR_2_H

