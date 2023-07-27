#include "Image.h"

#include <cassert>
#include <fstream>
#include <stdexcept>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

using std::vector;

Image::Image(std::string imagePath)
{
	int width, height;
	int temp;

	unsigned char* imageData = stbi_load(imagePath.c_str(), &width, &height, &temp, 3);
	assert(imageData);

	m_Colors.reserve(width * height);
	const unsigned imageDataSize = width * height * 3;

	m_Width = width;
	m_Height = height;

	for (int i = 0; i < imageDataSize; i += 3)
		m_Colors.emplace_back(imageData[i] / 255.0f, imageData[i + 1] / 255.0f, imageData[i + 2] / 255.0f);

	stbi_image_free(imageData);
}

Image::Image(unsigned width, unsigned height, Color color)
	: m_Width(width), m_Height(height), m_Colors(height * width, color)
{
}

void Image::setPixel(unsigned x, unsigned y, Color color)
{
	assert(x < m_Width && y < m_Height);
	m_Colors[m_Width * y + x] = color;
}

void Image::writeToPPM(const char* filePath)
{
	assert(filePath);

	std::ofstream outputStream(filePath);

	if (!outputStream || !outputStream.is_open())
		throw std::invalid_argument("Can not open file! Maybe it's being used!");

	const int bufferSize = 2048;
	const int colorStrSize = 3 * 3 + 2; //  3 digits per 3 color channels and 2 spaces
	const int maxBuffSize = bufferSize - colorStrSize;

	std::string buffer;
	buffer.reserve(bufferSize);

	outputStream << "P3\n" << m_Width << " " << m_Height << "\n" << "255\n";

	for (unsigned row = 0; row < m_Height; row++)
	{
		for (unsigned col = 0; col < m_Width; col++)
		{
			buffer += m_Colors[m_Width * row + col].toString() + "\n";

			if (buffer.size() >= maxBuffSize || (row + 1 >= m_Height && col + 1 >= m_Width))
			{
				outputStream << buffer;
				buffer.clear();
			}
		}
	}

	outputStream.close();
}

const Color& Image::getPixel(unsigned x, unsigned y) const
{
	return m_Colors[m_Width * y + x];
}

const unsigned Image::getWidth() const
{
	return m_Width;
}

const unsigned Image::getHeight() const
{
	return m_Height;
}

const std::vector<Color>& Image::getPixels() const
{
	return m_Colors;
}
