template<typename Type>
inline Vector2<Type>::Vector2()
	: m_X(Type{}), m_Y(Type{})
{
}

template<typename Type>
inline Vector2<Type>::Vector2(Type x, Type y)
	: m_X(x), m_Y(y)
{
}

template<typename Type>
template<typename U>
inline Vector2<Type>::Vector2(const Vector2<U>& other)
	: Vector2((Type)other.x(), (Type)other.y)
{
}

template<typename Type>
inline const Type& Vector2<Type>::x() const
{
	return m_X;
}

template<typename Type>
inline const Type& Vector2<Type>::y() const
{
	return m_Y;
}

template<typename Type>
inline Type& Vector2<Type>::x()
{
	return m_X;
}

template<typename Type>
inline Type& Vector2<Type>::y()
{
	return m_Y;
}

template<typename Type>
inline Vector2<Type> Vector2<Type>::operator-() const
{
	return Vector2(-m_X, -m_Y);
}

template<typename Type>
inline Vector2<Type>& Vector2<Type>::operator+=(Vector2 p)
{
	m_X += p.m_X;
	m_Y += p.m_Y;

	return *this;
}

template<typename Type>
inline Vector2<Type>& Vector2<Type>::operator-=(Vector2 p)
{
	*this += -p;
	return *this;
}

template<typename Type>
inline Vector2<Type>& Vector2<Type>::operator*=(Type scalar)
{
	m_X *= scalar;
	m_Y *= scalar;

	return *this;
}

template<typename Type>
inline Vector2<Type>& Vector2<Type>::operator/=(Type scalar)
{
	*this *= (1 / scalar);
	return *this;
}

template<typename Type>
inline Vector2<Type> operator+(Vector2<Type> p1, Vector2<Type> p2)
{
	return p1 += p2;
}

template<typename Type>
inline Vector2<Type> operator-(Vector2<Type> p1, Vector2<Type> p2)
{
	return p1 -= p2;
}

template<typename Type>
inline Vector2<Type> operator*(Vector2<Type> p, Type scalar)
{
	return p *= scalar;
}

template<typename Type>
inline Vector2<Type> operator*(Type scalar, Vector2<Type> p)
{
	return p *= scalar;
}

template<typename Type>
inline Vector2<Type> operator/(Vector2<Type> p, Type scalar)
{
	return p /= scalar;
}

template<typename Type>
inline bool operator==(Vector2<Type> p1, Vector2<Type> p2)
{
	return p1.x() == p2.x() && p1.y() == p2.y();
}

template<typename Type>
inline bool operator!=(Vector2<Type> p1, Vector2<Type> p2)
{
	return !(p1 == p2);
}

template<typename Type>
inline float dotProduct(Vector2<Type> p1, Vector2<Type> p2)
{
	return p1.x() * p2.x() + p1.y() * p2.y();
}