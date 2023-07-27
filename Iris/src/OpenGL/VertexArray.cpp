#include "VertexArray.h"

using namespace GL;

VertexArray::VertexArray()
{
    glGenVertexArrays(1, &m_ArrayId);
    glBindVertexArray(m_ArrayId);
}

VertexArray::~VertexArray()
{
    glDeleteVertexArrays(1, &m_ArrayId);
}

void VertexArray::bind() const
{
    glBindVertexArray(m_ArrayId);
}

void VertexArray::unbind() const
{
    glBindVertexArray(0);
}

void VertexArray::addVertexAttributes(const VertexBuffer& vertexBuffer)
{
    bind();
    int offset = 0;
    const unsigned& stride = vertexBuffer.getStride();

    for (const Attribute& attribute : vertexBuffer.getAttributes())
    {
        const GLenum& type = attribute.type;
        const unsigned& size = attribute.size;
        const unsigned& index = attribute.index;
        const unsigned typeSize = getTypeSize(type);

        glEnableVertexAttribArray(index);
        glVertexAttribPointer(index, size, type, GL_FALSE, stride, (const void*)offset);
        offset += typeSize * size;
    }
}
