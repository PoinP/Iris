#include "IndexBuffer.h"

using namespace GL;

IndexBuffer::IndexBuffer(const unsigned* data, unsigned size, GLenum usage)
	: m_Count(size / 4)
{
	glGenBuffers(1, &m_BufferId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_BufferId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, usage);
}

IndexBuffer::~IndexBuffer()
{
	glDeleteBuffers(1, &m_BufferId);
}

void IndexBuffer::bind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, m_BufferId);
}

void IndexBuffer::unbind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

unsigned IndexBuffer::getCount() const
{
    return m_Count;
}
