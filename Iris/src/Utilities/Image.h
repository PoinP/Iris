#ifndef IMAGE_H

#define IMAGE_H

#include <vector>

#include <Containers/Color.h>

class Image
{
public:
	Image(std::string imagePath);
	Image(unsigned width, unsigned height, Color color = { 0.0f, 0.0f, 0.0f });

	void setPixel(unsigned x, unsigned y, Color color);
	void writeToPPM(const char* filePath);

	const Color& getPixel(unsigned x, unsigned y) const;
	const unsigned getWidth() const;
	const unsigned getHeight() const;
	const std::vector<Color>& getPixels() const;

private:
	unsigned m_Width;
	unsigned m_Height;
	std::vector<Color> m_Colors;
};

#endif // !IMAGE_H