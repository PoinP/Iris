#ifndef MATRIX_4_H

#define MATRIX_4_H

#include <Containers/Matrix3.h>
#include <Containers/Vector4.h>

class Matrix4
{
public:
	Matrix4();
	/// Creates a matrix with 'scalar' used for the main diagonal
	Matrix4(const float& scalar);
	Matrix4(const Matrix3& mat);
	Matrix4(const Vector4f& c0, const Vector4f& c1, const Vector4f& c2, const Vector4f& c4);

	explicit operator Matrix3() const;

	Matrix4 operator-() const;

	const Vector4f& operator[](size_t colIdx) const;
	Vector4f& operator[](size_t colIdx);

	Matrix4& operator+=(const Matrix4& other);
	Matrix4& operator-=(const Matrix4& other);
	Matrix4& operator*=(float scalar);
	Matrix4& operator*=(const Matrix4& other);

	const Vector4f& col0() const;
	const Vector4f& col1() const;
	const Vector4f& col2() const;
	const Vector4f& col3() const;

	Vector4f& col0();
	Vector4f& col1();
	Vector4f& col2();
	Vector4f& col3();

	//Inverting a 4x4 is a pain in the a**
	//It probably won't be needed anyway ;P
	//void invert();
	void invertAffine();
	void transpose();
	float determinant() const;

private:
	Vector4f m_Cols[4];
};

Matrix4 operator+(Matrix4 lhs, const Matrix4& rhs);
Matrix4 operator-(Matrix4 lhs, const Matrix4& rhs);
Matrix4 operator*(Matrix4 lhs, const Matrix4& rhs);
Matrix4 operator*(Matrix4 mat, float scalar);
Matrix4 operator*(float scalar, Matrix4 mat);
Vector4f operator*(const Matrix4& mat, const Vector4f& vec);
Vector3f operator*(const Matrix4& mat, const Vector3f& vec);

Matrix4 invertAffine(const Matrix4& mat);
Matrix4 transpose(const Matrix4& mat);

#endif // !MATRIX_4_H

