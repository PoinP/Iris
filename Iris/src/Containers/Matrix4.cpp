#include "Matrix4.h"

Matrix4::Matrix4()
	: Matrix4(1.0f)
{
}

Matrix4::Matrix4(const float& scalar)
	: m_Cols { Vector4f(scalar, 0.0f, 0.0f, 0.0f),
			   Vector4f(0.0f, scalar, 0.0f, 0.0f),
			   Vector4f(0.0f, 0.0f, scalar, 0.0f),
			   Vector4f(0.0f, 0.0f, 0.0f, scalar) }
{
}

Matrix4::Matrix4(const Matrix3& mat)
	: m_Cols { Vector4f(mat.col0(), 0.0f),
			   Vector4f(mat.col1(), 0.0f),
			   Vector4f(mat.col2(), 0.0f),
			   Vector4f(0.0f, 0.0f, 0.0f, 1.0f) }
{
}

Matrix4::Matrix4(const Vector4f& c0, const Vector4f& c1, const Vector4f& c2, const Vector4f& c3)
	: m_Cols { c0, c1, c2, c3 }
{
}

Matrix4::operator Matrix3() const
{
	return Matrix3(Vector3f(m_Cols[0].x(), m_Cols[0].y(), m_Cols[0].z()),
				   Vector3f(m_Cols[1].x(), m_Cols[1].y(), m_Cols[1].z()),
				   Vector3f(m_Cols[2].x(), m_Cols[2].y(), m_Cols[2].z()));
}

Matrix4 Matrix4::operator-() const
{
	return Matrix4(-m_Cols[0], -m_Cols[1], -m_Cols[2], -m_Cols[3]);
}

const Vector4f& Matrix4::operator[](size_t colIdx) const
{
	assert(colIdx >= 0 && colIdx <= 3);
	return m_Cols[colIdx];
}

Vector4f& Matrix4::operator[](size_t colIdx)
{
	assert(colIdx >= 0 && colIdx <= 3);
	return m_Cols[colIdx];
}

Matrix4& Matrix4::operator+=(const Matrix4& other)
{
	m_Cols[0] += other.m_Cols[0];
	m_Cols[1] += other.m_Cols[1];
	m_Cols[2] += other.m_Cols[2];
	m_Cols[3] += other.m_Cols[3];

	return *this;
}

Matrix4& Matrix4::operator-=(const Matrix4& other)
{
	*this += -other;
	return *this;
}

Matrix4& Matrix4::operator*=(float scalar)
{
	m_Cols[0] *= scalar;
	m_Cols[1] *= scalar;
	m_Cols[2] *= scalar;
	m_Cols[3] *= scalar;

	return *this;
}

Matrix4& Matrix4::operator*=(const Matrix4& other)
{
	*this = Matrix4(*this * other.m_Cols[0], 
					*this * other.m_Cols[1], 
					*this * other.m_Cols[2], 
					*this * other.m_Cols[3]);

	return *this;
}

const Vector4f& Matrix4::col0() const
{
	return m_Cols[0];
}

const Vector4f& Matrix4::col1() const
{
	return m_Cols[1];
}

const Vector4f& Matrix4::col2() const
{
	return m_Cols[2];
}

const Vector4f& Matrix4::col3() const
{
	return m_Cols[3];
}

Vector4f& Matrix4::col0()
{
	return m_Cols[0];
}

Vector4f& Matrix4::col1()
{
	return m_Cols[1];
}

Vector4f& Matrix4::col2()
{
	return m_Cols[2];
}

Vector4f& Matrix4::col3()
{
	return m_Cols[3];
}

// ---- Invert Affine Explanation ----- //
/*
	inv(A) = [ inv(M)   -inv(M) * b ]
			 [   0            1     ]
	Where A - 4x4 Matrix
		  M - 3x3 Matrix
		  b - Column 4's 3x3 vector
*/
void Matrix4::invertAffine()
{
	Matrix3 upperMatrix = static_cast<Matrix3>(*this);
	upperMatrix.invert();

	Vector3f lastColumn = -upperMatrix * static_cast<Vector3f>(m_Cols[3]);

	*this = Matrix4(upperMatrix);
	m_Cols[3] = lastColumn;
}

void Matrix4::transpose()
{
	*this = Matrix4(
		Vector4f(m_Cols[0].x(), m_Cols[1].x(), m_Cols[2].x(), m_Cols[3].x()),
		Vector4f(m_Cols[0].y(), m_Cols[1].y(), m_Cols[2].y(), m_Cols[3].y()),
		Vector4f(m_Cols[0].z(), m_Cols[1].z(), m_Cols[2].z(), m_Cols[3].z()),
		Vector4f(m_Cols[0].w(), m_Cols[1].w(), m_Cols[2].w(), m_Cols[3].w())
	);
}

// Adjugate Matrix Method
float Matrix4::determinant() const
{
	Matrix4 temp = *this;
	temp.transpose();

	Matrix3 a0 = Matrix3(
		static_cast<Vector3f>(temp.col1()),
		static_cast<Vector3f>(temp.col2()),
		static_cast<Vector3f>(temp.col3())
	);

	Matrix3 a1 = Matrix3(
		static_cast<Vector3f>(temp.col0()),
		static_cast<Vector3f>(temp.col2()),
		static_cast<Vector3f>(temp.col3())
	);

	Matrix3 a2 = Matrix3(
		static_cast<Vector3f>(temp.col0()),
		static_cast<Vector3f>(temp.col1()),
		static_cast<Vector3f>(temp.col3())
	);

	Matrix3 a3 = Matrix3(
		static_cast<Vector3f>(temp.col0()),
		static_cast<Vector3f>(temp.col1()),
		static_cast<Vector3f>(temp.col2())
	);

	return -a0.determinant() + a1.determinant() - a2.determinant() + a3.determinant();
}

Matrix4 operator+(Matrix4 lhs, const Matrix4& rhs)
{
	return lhs += rhs;
}

Matrix4 operator-(Matrix4 lhs, const Matrix4& rhs)
{
	return lhs -= rhs;
}

Matrix4 operator*(Matrix4 lhs, const Matrix4& rhs)
{
	return lhs *= rhs;
}

Matrix4 operator*(Matrix4 mat, float scalar)
{
	return mat *= scalar;
}

Matrix4 operator*(float scalar, Matrix4 mat)
{
	return mat *= scalar;
}

Vector4f operator*(const Matrix4& mat, const Vector4f& vec)
{
	return Vector4f(
		((mat.col0().x() * vec.x()) + (mat.col1().x() * vec.y()) + (mat.col2().x() * vec.z()) + (mat.col3().x() * vec.w())),
		((mat.col0().y() * vec.x()) + (mat.col1().y() * vec.y()) + (mat.col2().y() * vec.z()) + (mat.col3().y() * vec.w())),
		((mat.col0().z() * vec.x()) + (mat.col1().z() * vec.y()) + (mat.col2().z() * vec.z()) + (mat.col3().z() * vec.w())),
		((mat.col0().w() * vec.x()) + (mat.col1().w() * vec.y()) + (mat.col2().w() * vec.z()) + (mat.col3().w() * vec.w()))
	);
}

Vector3f operator*(const Matrix4& mat, const Vector3f& vec)
{
	return Vector3f(
		((mat.col0().x() * vec.x()) + (mat.col1().x() * vec.y()) + (mat.col2().x() * vec.z()) + mat.col3().x()),
		((mat.col0().y() * vec.x()) + (mat.col1().y() * vec.y()) + (mat.col2().y() * vec.z()) + mat.col3().y()),
		((mat.col0().z() * vec.x()) + (mat.col1().z() * vec.y()) + (mat.col2().z() * vec.z()) + mat.col3().z())
	);
}

// ---- Invert Affine Explanation ----- //
/*
	inv(A) = [ inv(M)   -inv(M) * b ]
			 [   0            1     ]
	Where A - 4x4 Matrix
		  M - 3x3 Matrix
		  b - Column 4's 3x3 vector
*/
Matrix4 invertAffine(const Matrix4& mat)
{
	Matrix3 upperMatrix = static_cast<Matrix3>(mat);
	upperMatrix.invert();

	Vector3f lastColumn = -upperMatrix * static_cast<Vector3f>(mat.col3());

	Matrix4 invertedMatrix = Matrix4(upperMatrix);
	invertedMatrix.col3() = lastColumn;

	return invertedMatrix;
}

Matrix4 transpose(const Matrix4& mat)
{
	return Matrix4(
		Vector4f(mat.col0().x(), mat.col1().x(), mat.col2().x(), mat.col3().x()),
		Vector4f(mat.col0().y(), mat.col1().y(), mat.col2().y(), mat.col3().y()),
		Vector4f(mat.col0().z(), mat.col1().z(), mat.col2().z(), mat.col3().z()),
		Vector4f(mat.col0().w(), mat.col1().w(), mat.col2().w(), mat.col3().w())
	);
}
