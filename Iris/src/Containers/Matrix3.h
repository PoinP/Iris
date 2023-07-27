#ifndef MATRIX_3_H

#define MATRIX_3_H

#include <Containers/Vector3.h>

class Matrix3
{
public:
	Matrix3();
	/// Creates a matrix with 'scalar' used for the main diagonal
	Matrix3(float scalar);
	Matrix3(const Vector3f& c0, const Vector3f& c1, const Vector3f& c2);

	Matrix3 operator-() const;

	const Vector3f& operator[](size_t colIdx) const;
	Vector3f& operator[](size_t colIdx);

	Matrix3& operator+=(const Matrix3& other);
	Matrix3& operator-=(const Matrix3& other);
	Matrix3& operator*=(float scalar);
	Matrix3& operator*=(const Matrix3& other);

	const Vector3f& col0() const;
	const Vector3f& col1() const;
	const Vector3f& col2() const;

	Vector3f& col0();
	Vector3f& col1();
	Vector3f& col2();

	void invert();
	void transpose();
	float determinant() const;

private:
	Vector3f m_Cols[3];
};

Matrix3 operator+(Matrix3 lhs, const Matrix3& rhs);
Matrix3 operator-(Matrix3 lhs, const Matrix3& rhs);
Matrix3 operator*(Matrix3 lhs, const Matrix3& rhs);
Matrix3 operator*(Matrix3 mat, float scalar);
Matrix3 operator*(float scalar, Matrix3 mat);
Vector3f operator*(const Matrix3& mat, const Vector3f& vec);

Matrix3 invert(const Matrix3& mat);
Matrix3 transpose(const Matrix3& mat);

#endif // !MATRIX_3_H