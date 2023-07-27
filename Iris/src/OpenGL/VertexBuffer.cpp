#include "VertexBuffer.h"

#include <cassert>

using namespace GL;

VertexBuffer::VertexBuffer(const void* data, unsigned size, GLenum usage)
	: m_Stride(0)
{
	glGenBuffers(1, &m_BufferId);
	glBindBuffer(GL_ARRAY_BUFFER, m_BufferId);
	glBufferData(GL_ARRAY_BUFFER, size, data, usage);
}

VertexBuffer::~VertexBuffer()
{
	glDeleteBuffers(1, &m_BufferId);
}

void VertexBuffer::bind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, m_BufferId);
}

void VertexBuffer::unbind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

unsigned VertexBuffer::getStride() const
{
	return m_Stride;
}

void VertexBuffer::addAttribute(const Attribute& attribute)
{
	addAttribute(attribute.index, attribute.size, attribute.type);
}

void VertexBuffer::addAttribute(unsigned index, unsigned size, GLenum type)
{
	assert(size > 0 && size < 4);
	m_Attributes.push_back(Attribute{ index, size, type });
	m_Stride += getTypeSize(type) * size;
}

const std::vector<Attribute>& VertexBuffer::getAttributes() const
{
	return m_Attributes;
}

unsigned GL::getTypeSize(GLenum type)
{
	switch (type)
	{
	case GL_BYTE:
	case GL_UNSIGNED_BYTE:
		return 1;

	case GL_SHORT:
	case GL_UNSIGNED_SHORT:
	case GL_HALF_FLOAT:
		return 2;

	case GL_INT:
	case GL_UNSIGNED_INT:
	case GL_FLOAT:
		return 4;

	case GL_DOUBLE:
		return 8;
	}

	assert(false);
}
