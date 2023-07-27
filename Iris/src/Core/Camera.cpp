#include "Camera.h"

#include <Utilities/Utility.h>

#include <cmath>
#include <cassert>

Camera::Camera(Point3f origin, unsigned width, unsigned height, float fov, float focalLength, float apertureSize)
    : m_Origin(origin), m_Width(width), m_Height(height), m_FOV(fromDegreesToRadians(fov)),
    m_FocalLength(focalLength), m_ApertureSize(apertureSize)
{
    assert(m_Height != 0);
    assert(m_FOV > 0.0f);
    assert(m_FocalLength > 0.0f);
    m_AspectRatio = (float)m_Width / m_Height;
}

Ray Camera::getRay(float x, float y) const
{
    Vector2f pixel(x, y);
    toNDC(pixel);
    toScreenSpace(pixel);

    Vector3f direction(pixel.x(), pixel.y(), -1.0f);
    direction = m_InverseView * direction;
    direction.normalize();

    const Point3f focalPoint = m_Origin + direction * (float)m_FocalLength;
    const Point3f camOrigin = m_Origin + randomVector(-0.5f, 0.5f) * (float)m_ApertureSize;
    direction = toUnitVector(focalPoint - camOrigin);

    return Ray(camOrigin, direction);
}

void Camera::generateRays(unsigned x, unsigned y, std::vector<Ray>& rays, unsigned sampleCount) const
{
    assert(sampleCount > 0);

    rays.emplace_back(getRay(x + 0.5f, y + 0.5f));

    for (int i = 1; i < sampleCount; i++)
    {
        const float xCoord = x + randomFloat(0, 0.9999f);
        const float yCoord = y + randomFloat(0, 0.9999f);

        rays.emplace_back(getRay(xCoord, yCoord));
    }
}

void Camera::setFOV(float degrees)
{
    assert(degrees > 0.f && degrees < 360.f);
    m_FOV = fromDegreesToRadians(degrees);
}

void Camera::setOrigin(const Point3f& origin)
{
    m_Origin = origin;
}

void Camera::setApertureSize(float apertureSize)
{
    m_ApertureSize = apertureSize;
}

void Camera::setFocalLength(float focalLength)
{
    m_FocalLength = focalLength;
}

void Camera::setResolution(unsigned width, unsigned height)
{
    assert(width > 0 && height > 0);
    m_Width = width;
    m_Height = height;
    m_AspectRatio = (float)m_Width / m_Height;
}

void Camera::lookAt(const Point3f& point)
{
    Vector3f forward = toUnitVector(m_Origin - point);
    Vector3f upTemp(0.0f, 1.0f, 0.0f);
    Vector3f right = toUnitVector(crossProduct(upTemp, forward));
    Vector3f up = toUnitVector(crossProduct(forward, right));

    m_View = Matrix3(right, up, forward);
    m_InverseView = invert(m_View);
}

void Camera::transform(const Matrix3& transformationMatrix)
{
    m_View *= transformationMatrix;
    m_InverseView = invert(m_View);
}

void Camera::pan(float degrees)
{
    float rad = fromDegreesToRadians(degrees);

    float cosTheta = cos(rad);
    float sinTheta = sin(rad);

    Matrix3 roataionMatrix(
        Vector3f(cosTheta, 0.0f, -sinTheta),
        Vector3f(0.0f, 1.0f, 0.0f),
        Vector3f(sinTheta, 0.0f, cosTheta)
    );

    m_View *= roataionMatrix;
    m_InverseView = invert(m_View);
}

void Camera::tilt(float degrees)
{
    float rad = fromDegreesToRadians(degrees);

    float cosTheta = cos(rad);
    float sinTheta = sin(rad);

    Matrix3 roataionMatrix(
        Vector3f(1.0f, 0.0f, 0.0f),
        Vector3f(0.0f, cosTheta, sinTheta),
        Vector3f(0.0f, -sinTheta, cosTheta)
    );

    m_View *= roataionMatrix;
    m_InverseView = invert(m_View);
}

void Camera::roll(float degrees)
{
    float rad = fromDegreesToRadians(degrees);

    float cosTheta = cos(rad);
    float sinTheta = sin(rad);

    Matrix3 roataionMatrix(
        Vector3f(cosTheta, sinTheta, 0.0f),
        Vector3f(-sinTheta, cosTheta, 0.0f),
        Vector3f(0.0f, 0.0f, 1.0f)
    );

    m_View *= roataionMatrix;
    m_InverseView = invert(m_View);
}

void Camera::dolly(float offset)
{
    m_Origin += m_View.col2() * offset;
}

void Camera::boom(float offset)
{
    m_Origin += m_View.col1() * offset;
}

void Camera::truck(float offset)
{
    m_Origin += m_View.col0() * offset;
}

void Camera::rotateOriginX(float degrees, const Point3f& center)
{
    float rad = fromDegreesToRadians(degrees);

    float cosTheta = cos(rad);
    float sinTheta = sin(rad);

    Matrix3 rotationMatrix(
        Vector3f(1.0f, 0.0f, 0.0f),
        Vector3f(0.0f, cosTheta, -sinTheta),
        Vector3f(0.0f, -sinTheta, cosTheta)
    );

    m_Origin -= center;
    m_Origin = rotationMatrix * m_Origin;
    m_Origin += center;
}

void Camera::rotateOriginY(float degrees, const Point3f& center)
{
    float rad = fromDegreesToRadians(degrees);

    float cosTheta = cos(rad);
    float sinTheta = sin(rad);

    Matrix3 rotationMatrix(
        Vector3f(cosTheta, 0.0f, -sinTheta),
        Vector3f(0.0f, 1.0f, 0.0f),
        Vector3f(sinTheta, 0.0f, cosTheta)
    );

    m_Origin -= center;
    m_Origin = rotationMatrix * m_Origin;
    m_Origin += center;
}

void Camera::rotateOriginZ(float degrees, const Point3f& center)
{
    float rad = fromDegreesToRadians(degrees);

    float cosTheta = cos(rad);
    float sinTheta = sin(rad);

    Matrix3 rotationMatrix(
        Vector3f(cosTheta, sinTheta, 0.0f),
        Vector3f(-sinTheta, cosTheta, 0.0f),
        Vector3f(0.0f, 0.0f, 1.0f)
    );

    m_Origin -= center;
    m_Origin = rotationMatrix * m_Origin;
    m_Origin += center;
}

void Camera::toNDC(Vector2f& pixel) const
{
    pixel.x() /= (m_Width - 1);
    pixel.y() /= (m_Height - 1);
}

void Camera::toScreenSpace(Vector2f& ndc) const
{
    ndc.x() = (2.0f * ndc.x()) - 1.0f;
    ndc.y() = 1.0f - (2.0f * ndc.y());

    static float fovOffset = tan(m_FOV / 2.0f);

    ndc.x() *= m_AspectRatio * fovOffset;
    ndc.y() *= fovOffset;
}