#ifndef GL_TEXTURE_H

#define GL_TEXTURE_H

#include <string>
#include <Utilities/Image.h>

namespace GL
{
	class Texture
	{
	public:
		Texture(const Image& image);
		Texture(const Texture&) = delete;
		~Texture();

		Texture& operator=(const Texture&) = delete;

		void update(const Image& image);

		void bind(unsigned slot = 0) const;
		void unbind() const;

	private:
		unsigned m_TexId;
		unsigned m_Width;
		unsigned m_Height;

		void setParameters();
	};
}

#endif // !GL_TEXTURE_H