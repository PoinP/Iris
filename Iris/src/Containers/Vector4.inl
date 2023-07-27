template<typename Type>
inline Vector4<Type>::Vector4()
	: m_Coords{ Type{}, Type{}, Type{}, Type{} }
{
}

template<typename Type>
inline Vector4<Type>::Vector4(const Vector2<Type>& vec2, const Type& z, const Type& w)
	: m_Coords{ vec2.x(), vec2.y(), z, w }
{
}

template<typename Type>
inline Vector4<Type>::Vector4(const Type& x, const Type& y, const Vector2<Type>& vec2)
	: m_Coords{ x, y , vec2.x(), vec2.y() }
{
}

template<typename Type>
inline Vector4<Type>::Vector4(const Vector3<Type>& vec3, const Type& w)
	: m_Coords{ vec3.x(), vec3.y(), vec3.z(), w }
{
}

template<typename Type>
inline Vector4<Type>::Vector4(const Type& x, const Vector3<Type>& vec3)
	: m_Coords{ x, vec3.x(), vec3.y(), vec3.z() }
{
}

template<typename Type>
inline Vector4<Type>::Vector4(const Type& x, const Type& y, const Type& z, const Type& w)
	: m_Coords{ x, y, z, w }
{
}

template<typename Type>
template<typename U>
inline Vector4<Type>::Vector4(const Vector4<U>& other)
	: Vector4((Type)other.x(), (Type)other.y(), (Type)other.z(), (Type)other.w())
{
}

template<typename Type>
inline Vector4<Type>::operator Vector3f() const
{
	return Vector3f(m_Coords[0], m_Coords[1], m_Coords[2]);
}

template<typename Type>
inline Vector4<Type> Vector4<Type>::operator-() const
{
	return Vector4(-m_Coords[0], -m_Coords[1], -m_Coords[2], -m_Coords[3]);
}

template<typename Type>
inline const Type& Vector4<Type>::operator[](size_t idx) const
{
	assert(idx >= 0 && idx <= 3);
	return m_Coords[idx];
}

template<typename Type>
inline Type& Vector4<Type>::operator[](size_t idx)
{
	assert(idx >= 0 && idx <= 3);
	return m_Coords[idx];
}

template<typename Type>
inline Vector4<Type>& Vector4<Type>::operator+=(const Vector4& other)
{
	m_Coords[0] += other.m_Coords[0];
	m_Coords[1] += other.m_Coords[1];
	m_Coords[2] += other.m_Coords[2];
	m_Coords[3] += other.m_Coords[3];

	return *this;
}

template<typename Type>
inline Vector4<Type>& Vector4<Type>::operator-=(const Vector4& other)
{
	*this += -other;
	return *this;
}

template<typename Type>
inline Vector4<Type>& Vector4<Type>::operator*=(Type scalar)
{
	m_Coords[0] *= scalar;
	m_Coords[1] *= scalar;
	m_Coords[2] *= scalar;
	m_Coords[3] *= scalar;

	return *this;
}

template<typename Type>
inline Vector4<Type>& Vector4<Type>::operator/=(Type scalar)
{
	*this *= (1 / scalar);
	return *this;
}

template<typename Type>
inline const Type& Vector4<Type>::x() const
{
	return m_Coords[0];
}

template<typename Type>
inline const Type& Vector4<Type>::y() const
{
	return m_Coords[1];
}

template<typename Type>
inline const Type& Vector4<Type>::z() const
{
	return m_Coords[2];
}

template<typename Type>
inline const Type& Vector4<Type>::w() const
{
	return m_Coords[3];
}

template<typename Type>
inline Type& Vector4<Type>::x()
{
	return m_Coords[0];
}

template<typename Type>
inline Type& Vector4<Type>::y()
{
	return m_Coords[1];
}

template<typename Type>
inline Type& Vector4<Type>::z()
{
	return m_Coords[2];
}

template<typename Type>
inline Type& Vector4<Type>::w()
{
	return m_Coords[3];
}

template<typename Type>
inline void Vector4<Type>::normalize()
{
	*this = toUnitVector(*this);
}

template<typename Type>
inline Type Vector4<Type>::length() const
{
	return sqrt(lengthSquared());
}

template<typename Type>
inline Type Vector4<Type>::lengthSquared() const
{
	return dotProduct(*this, *this);
}

template<typename Type>
inline Vector4<Type> operator+(Vector4<Type> lhs, const Vector4<Type>& rhs)
{
	return lhs += rhs;
}

template<typename Type>
inline Vector4<Type> operator-(Vector4<Type> lhs, const Vector4<Type>& rhs)
{
	return lhs -= rhs;
}

template<typename Type>
inline Vector4<Type> operator*(Vector4<Type> lhs, Type scalar)
{
	return lhs *= scalar;
}

template<typename Type>
inline Vector4<Type> operator*(Type scalar, Vector4<Type> rhs)
{
	return rhs *= scalar;
}

template<typename Type>
inline Vector4<Type> operator/(Vector4<Type> lhs, Type scalar)
{
	return lhs /= scalar;
}

template<typename Type>
inline bool operator==(const Vector4<Type>& lhs, const Vector4<Type>& rhs)
{
	return lhs.x() == rhs.x() &&
		   lhs.y() == rhs.y() &&
		   lhs.z() == rhs.z() &&
		   lhs.w() == rhs.w();
}

template<typename Type>
inline bool operator!=(const Vector4<Type>& lhs, const Vector4<Type>& rhs)
{
	return !(lhs == rhs);
}

template<typename Type>
inline Type dotProduct(const Vector4<Type>& lhs, const Vector4<Type>& rhs)
{
	return lhs.x() * rhs.x() +
		   lhs.y() * rhs.y() +
		   lhs.z() * rhs.z() +
		   lhs.w() * rhs.w();
}

template<typename Type>
inline Vector4<Type> toUnitVector(const Vector4<Type>& vec)
{
	const Type length = vec.length();
	assert(length > 0);
	return vec * (1 / length);
}
