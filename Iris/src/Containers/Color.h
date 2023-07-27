#ifndef COLOR_H

#define COLOR_H

#include <Containers/Vector3.h>
#include <Containers/Vector4.h>

#include <iostream>
#include <string>

class Color
{
public:
	Color();
	Color(float red, float green, float blue, float alpha = 1.0f);
	explicit Color(const Vector3f colors);
	explicit Color(const Vector4f colors);

	float r() const;
	float g() const;
	float b() const;
	float a() const;

	float& r();
	float& g();
	float& b();
	float& a();

	std::string toString(bool ignoreAlpha = true) const;

	static Color generateRandomColor(unsigned char threshold = 255);
	static Color generateRandomBasicColor();


	Color& operator+= (const Color& other);
	Color& operator-= (const Color& other);
	Color& operator*= (const Color& other);
	Color& operator*= (float scalar);
	Color& operator/= (float scalar);

private:
	float m_Red;
	float m_Green;
	float m_Blue;
	float m_Alpha;
};

std::ostream& operator<<(std::ostream& stream, Color color);

Color operator+ (Color lhs, const Color& rhs);
Color operator- (Color lhs, const Color& rhs);
Color operator* (Color lhs, const Color& rhs);

Color operator* (Color lhs, float scalar);
Color operator* (float scalar, Color rhs);
Color operator/ (Color lhs, float scalar);
Color operator/ (float scalar, const Color& rhs);

bool operator== (const Color& lhs, const Color& rhs);
bool operator!= (const Color& lhs, const Color& rhs);

Color clamp(Color color, float min, float max);

#endif // !COLOR_H