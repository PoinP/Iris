#include "Matrix3.h"

#include <cassert>

Matrix3::Matrix3()
    : Matrix3(1.0f)
{
}

Matrix3::Matrix3(float scalar)
    : m_Cols { Vector3f(scalar, 0.0f, 0.0f),
              Vector3f(0.0f, scalar, 0.0f),
              Vector3f(0.0f, 0.0f, scalar) }
{
}

Matrix3::Matrix3(const Vector3f& c0, const Vector3f& c1, const Vector3f& c2)
    : m_Cols { c0, c1, c2 }
{
}

Matrix3 Matrix3::operator-() const
{
    return Matrix3(-m_Cols[0], -m_Cols[1], -m_Cols[2]);
}

const Vector3f& Matrix3::operator[](size_t colIdx) const
{
    assert(colIdx >= 0 && colIdx <= 2);
    return m_Cols[colIdx];
}

Vector3f& Matrix3::operator[](size_t colIdx)
{
    assert(colIdx >= 0 && colIdx <= 2);
    return m_Cols[colIdx];
}

Matrix3& Matrix3::operator+=(const Matrix3& other)
{
    m_Cols[0] += other.m_Cols[0];
    m_Cols[1] += other.m_Cols[1];
    m_Cols[2] += other.m_Cols[2];

    return *this;
}

Matrix3& Matrix3::operator-=(const Matrix3& other)
{
    *this += -other;
    return *this;
}

Matrix3& Matrix3::operator*=(float scalar)
{
    m_Cols[0] *= scalar;
    m_Cols[1] *= scalar;
    m_Cols[2] *= scalar;

    return *this;
}

Matrix3& Matrix3::operator*=(const Matrix3& other)
{
    *this = Matrix3(*this * other.m_Cols[0], 
                    *this * other.m_Cols[1], 
                    *this * other.m_Cols[2]);
    return *this;
}

const Vector3f& Matrix3::col0() const
{
    return m_Cols[0];
}

const Vector3f& Matrix3::col1() const
{
    return m_Cols[1];
}

const Vector3f& Matrix3::col2() const
{
    return m_Cols[2];
}

Vector3f& Matrix3::col0()
{
    return m_Cols[0];
}

Vector3f& Matrix3::col1()
{
    return m_Cols[1];
}

Vector3f& Matrix3::col2()
{
    return m_Cols[2];
}

void Matrix3::invert()
{
    float det = determinant();
    assert(det != 0.0f);

    float inverseDet = 1.0f / det;

    Vector3f v0, v1, v2;

    v0 = crossProduct(m_Cols[1], m_Cols[2]) * inverseDet;
    v1 = crossProduct(m_Cols[2], m_Cols[0]) * inverseDet;
    v2 = crossProduct(m_Cols[0], m_Cols[1]) * inverseDet;

    m_Cols[0] = v0;
    m_Cols[1] = v1;
    m_Cols[2] = v2;
}

void Matrix3::transpose()
{
    *this = Matrix3(
        Vector3f(m_Cols[0].x(), m_Cols[1].x(), m_Cols[2].x()),
        Vector3f(m_Cols[0].y(), m_Cols[1].y(), m_Cols[2].y()),
        Vector3f(m_Cols[0].z(), m_Cols[1].z(), m_Cols[2].z())
    );
}

float Matrix3::determinant() const
{
    return dotProduct(m_Cols[0], crossProduct(m_Cols[1], m_Cols[2]));
}

Matrix3 operator+(Matrix3 lhs, const Matrix3& rhs)
{
    return lhs += rhs;
}

Matrix3 operator-(Matrix3 lhs, const Matrix3& rhs)
{
    return lhs -= rhs;
}

Matrix3 operator*(Matrix3 lhs, const Matrix3& rhs)
{
    return lhs *= rhs;
}

Matrix3 operator*(Matrix3 mat, float scalar)
{
    return mat *= scalar;
}

Matrix3 operator*(float scalar, Matrix3 mat)
{
    return mat *= scalar;
}

Vector3f operator*(const Matrix3& mat, const Vector3f& vec)
{
    return Vector3f(
        ((mat.col0().x() * vec.x()) + (mat.col1().x() * vec.y()) + (mat.col2().x() * vec.z())),
        ((mat.col0().y() * vec.x()) + (mat.col1().y() * vec.y()) + (mat.col2().y() * vec.z())),
        ((mat.col0().z() * vec.x()) + (mat.col1().z() * vec.y()) + (mat.col2().z() * vec.z()))
    );
}

Matrix3 invert(const Matrix3& mat)
{
    float det = mat.determinant();

    assert(det != 0.0f);

    Matrix3 inverse;
    float inverseDet = 1.0f / det;

    inverse.col0() = crossProduct(mat.col1(), mat.col2()) * inverseDet;
    inverse.col1() = crossProduct(mat.col2(), mat.col0()) * inverseDet;
    inverse.col2() = crossProduct(mat.col0(), mat.col1()) * inverseDet;

    return inverse;
}

Matrix3 transpose(const Matrix3& mat)
{
    return Matrix3(
        Vector3f(mat.col0().x(), mat.col1().x(), mat.col2().x()),
        Vector3f(mat.col0().y(), mat.col1().y(), mat.col2().y()),
        Vector3f(mat.col0().z(), mat.col1().z(), mat.col2().z())
    );
}
