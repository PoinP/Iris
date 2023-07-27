#ifndef IMAGE_TEXTURE_H

#define IMAGE_TEXTURE_H

#include <Utilities/Image.h>
#include <Objects/Texture.h>

class ImageTexture : public Texture
{
public:
	ImageTexture(std::string imagePath)
		: m_ImageTexture(imagePath){}

	virtual Color getColor(float u, float v) const override
	{
		unsigned width = (unsigned)floor(fabs(v) * (m_ImageTexture.getWidth() - 1));
		unsigned height = (unsigned)floor(fabs(u) * (m_ImageTexture.getHeight() - 1));

		return m_ImageTexture.getPixel(height, width);
	}

private:
	Image m_ImageTexture;
};

#endif // !IMAGE_TEXTURE_H
