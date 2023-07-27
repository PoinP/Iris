#include "Texture.h"

#include <glad/glad.h>

using namespace GL;

Texture::Texture(const Image& image)
{
	glGenTextures(1, &m_TexId);
	glBindTexture(GL_TEXTURE_2D, m_TexId);

	m_Width = image.getWidth();
	m_Height = image.getHeight();

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_Width, m_Height, 0, GL_RGBA, GL_FLOAT, image.getPixels().data());
	setParameters();

	glBindBuffer(GL_TEXTURE_2D, 0);
}

Texture::~Texture()
{
	glDeleteTextures(1, &m_TexId);
}

void GL::Texture::update(const Image& image)
{
	assert(image.getHeight() == m_Height && image.getWidth() == m_Width);

	glBindTexture(GL_TEXTURE_2D, m_TexId);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_Width, m_Height, GL_RGBA, GL_FLOAT, image.getPixels().data());
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::bind(unsigned slot) const
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, m_TexId);
}

void Texture::unbind() const
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::setParameters()
{
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}
