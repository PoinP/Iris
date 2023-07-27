#ifndef GL_VERTEX_ARRAY_H

#define GL_VERTEX_ARRAY_H

#include <glad/glad.h>
#include <OpenGL/VertexBuffer.h>
#include <OpenGL/IndexBuffer.h>

namespace GL
{
	class VertexArray
	{
	public:
		VertexArray();
		VertexArray(const VertexArray&) = delete;
		~VertexArray();

		VertexArray& operator= (const VertexArray&) = delete;

		void bind() const;
		void unbind() const;

		void addVertexAttributes(const GL::VertexBuffer& vertexBuffer);

	private:
		unsigned m_ArrayId;
	};
}

#endif // !GL_VERTEX_ARRAY_H
