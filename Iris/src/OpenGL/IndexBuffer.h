#ifndef GL_INDEX_BUFFER_H

#define GL_INDEX_BUFFER_H

#include <glad/glad.h>

namespace GL
{
	class IndexBuffer
	{
	public:
		// Usage can be one of the following:
		// GL_STREAM_DRAW, GL_STREAM_READ, GL_STREAM_COPY, GL_STATIC_DRAW, 
		// GL_STATIC_READ, GL_STATIC_COPY, GL_DYNAMIC_DRAW, GL_DYNAMIC_READ, 
		// or GL_DYNAMIC_COPY.
		IndexBuffer(const unsigned* data, unsigned size, GLenum usage = GL_STATIC_DRAW);
		IndexBuffer(const IndexBuffer&) = delete;
		~IndexBuffer();

		IndexBuffer& operator=(const IndexBuffer&) = delete;

		void bind() const;
		void unbind() const;

		unsigned getCount() const;

	private:
		unsigned m_BufferId;
		unsigned m_Count;
	};
}


#endif // !GL_INDEX_BUFFER_H
