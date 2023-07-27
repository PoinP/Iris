#ifndef GL_RENDERER_H

#define GL_RENDERER_H

#include <OpenGL/VertexArray.h>
#include <OpenGL/IndexBuffer.h>
#include <OpenGL/Shader.h>

namespace GL
{
	class GLRenderer
	{
	public:
		static void clear();
		static void draw(const GL::VertexArray& va, const GL::IndexBuffer& ib, const GL::Shader& shader);
		static void draw(const GL::VertexArray& va, int count, const GL::Shader& shader);
	};
}

#endif // !GL_RENDERER_H