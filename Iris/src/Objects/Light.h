#ifndef LIGHT_H

#define LIGHT_H

#include <Containers/Vector3.h>

class Light
{
public:
	Light(const Vector3f& position, float intensity)
		: m_Position(position), m_Intensity(intensity < 0 ? 0 : intensity)
	{}

	const Vector3f& position() const { return m_Position; }
	float intensity() const			 { return m_Intensity; }

private:
	Vector3f m_Position;
	float m_Intensity;
};

#endif // !LIGHT_H
