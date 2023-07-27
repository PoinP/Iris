#include "Color.h"

#include "Utilities/Utility.h"

Color::Color()
	: m_Red(0.0f), m_Green(0.0f), m_Blue(0.0f), m_Alpha(1.0f)
{
}

Color::Color(float red, float green, float blue, float alpha)
	: m_Red(red), m_Green(green), m_Blue(blue), m_Alpha(alpha)
{
}

Color::Color(const Vector3f colors)
	: Color(colors.x(), colors.y(), colors.z())
{
}

Color::Color(const Vector4f colors)
	: Color(colors.x(), colors.y(), colors.z(), colors.w())
{
}

float Color::r() const
{
	return m_Red;
}

float Color::g() const
{
	return m_Green;
}

float Color::b() const
{
	return m_Blue;
}

float Color::a() const
{
	return m_Alpha;
}

float& Color::r()
{
	return m_Red;
}

float& Color::g()
{
	return m_Green;
}

float& Color::b()
{
	return m_Blue;
}

float& Color::a()
{
	return m_Alpha;
}

std::string Color::toString(bool ignoreAlpha) const
{
	using std::to_string;

	return ignoreAlpha ?
		to_string((int)(r() * 255)) + " " + to_string((int)(g() * 255)) + " " + to_string((int)(b() * 255)) :
		to_string((int)(r() * 255)) + " " + to_string((int)(g() * 255)) + " " + to_string((int)(b() * 255)) + " " + to_string((int)(a() * 255));
}

Color Color::generateRandomColor(unsigned char threshold)
{
	return Color(
		randomFloat(0.0f, threshold / 255.0f),
		randomFloat(0.0f, threshold / 255.0f),
		randomFloat(0.0f, threshold / 255.0f)
	);
}

Color Color::generateRandomBasicColor()
{
	const char RED = 0x1;
	const char GREEN = 0x2;
	const char BLUE = 0x4;

	char colorIndex = randomInt(1, 6); // 0 is black and 7 is white so they are excluded

	Color randomColor;
	if (colorIndex & RED)
		randomColor.m_Red = 1.0f;
	if (colorIndex & GREEN)
		randomColor.m_Green = 1.0f;
	if (colorIndex & BLUE)
		randomColor.m_Blue = 1.0f;

	return randomColor;
}

Color& Color::operator+=(const Color& other)
{
	m_Red += other.m_Red;
	m_Green += other.m_Green;
	m_Blue += other.m_Blue;
	m_Alpha += other.m_Alpha;

	return *this;
}

Color& Color::operator-=(const Color& other)
{
	m_Red -= other.m_Red;
	m_Green -= other.m_Green;
	m_Blue -= other.m_Blue;
	m_Alpha -= other.m_Alpha;

	return *this;
}

Color& Color::operator*=(const Color& other)
{
	m_Red *= other.m_Red;
	m_Green *= other.m_Green;
	m_Blue *= other.m_Blue;
	m_Alpha *= other.m_Alpha;

	return *this;
}

Color& Color::operator*=(float scalar)
{
	m_Red *= scalar;
	m_Green *= scalar;
	m_Blue *= scalar;
	m_Alpha *= scalar;

	return *this;
}

Color& Color::operator/=(float scalar)
{
	float invertedScalar = 1 / scalar;
	*this *= invertedScalar;
	return *this;
}

std::ostream& operator<<(std::ostream& stream, Color color)
{
	stream << (int)(color.r() * 255)<< " " << (int)(color.g() * 255) << " " << (int)(color.b() * 255);
	return stream;
}

Color operator+(Color lhs, const Color& rhs)
{
	return lhs += rhs;
}

Color operator-(Color lhs, const Color& rhs)
{
	return lhs -= rhs;
}

Color operator*(Color lhs, const Color& rhs)
{
	return lhs *= rhs;
}

Color operator*(Color lhs, float scalar)
{
	return lhs *= scalar;
}

Color operator*(float scalar, Color rhs)
{
	return rhs *= scalar;
}

Color operator/(Color lhs, float scalar)
{
	return lhs /= scalar;
}

Color operator/(float scalar, const Color& rhs)
{
	return Color(scalar / rhs.r(), scalar / rhs.g(), scalar / rhs.b(), scalar / rhs.a());;
}

bool operator==(const Color& lhs, const Color& rhs)
{
	return lhs.r() == rhs.r() && 
		   lhs.g() == rhs.g() && 
		   lhs.b() == rhs.b() && 
		   rhs.a() == lhs.a();
}

bool operator!=(const Color& lhs, const Color& rhs)
{
	return !(lhs == rhs);
}

Color clamp(Color color, float min, float max)
{
	color.r() = clamp(color.r(), min, max);
	color.g() = clamp(color.g(), min, max);
	color.b() = clamp(color.b(), min, max);
	color.a() = clamp(color.a(), min, max);

	return color;
}