#ifndef GL_VERTEX_BUFFER_H

#define GL_VERTEX_BUFFER_H

#include <vector>
#include <glad/glad.h>

namespace GL
{
	struct Attribute
	{
		unsigned index;
		unsigned size;
		GLenum type;
	};

	class VertexBuffer
	{
	public:
		// Usage can be one of the following:
		// GL_STREAM_DRAW, GL_STREAM_READ, GL_STREAM_COPY, GL_STATIC_DRAW, 
		// GL_STATIC_READ, GL_STATIC_COPY, GL_DYNAMIC_DRAW, GL_DYNAMIC_READ, 
		// or GL_DYNAMIC_COPY.
		VertexBuffer(const void* data, unsigned size, GLenum usage = GL_STATIC_DRAW);
		VertexBuffer(const VertexBuffer&) = delete;
		~VertexBuffer();

		VertexBuffer& operator= (const VertexBuffer&) = delete;


		void bind() const;
		void unbind() const;
		unsigned getStride() const;

		void addAttribute(const Attribute& attribute);
		void addAttribute(unsigned index, unsigned size, GLenum type);
		const std::vector<Attribute>& getAttributes() const;

	private:
		unsigned m_Stride;
		unsigned m_BufferId;
		std::vector<Attribute> m_Attributes;
	};

	unsigned getTypeSize(GLenum type);
}

#endif // !GL_VERTEX_BUFFER_H
