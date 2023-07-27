#include "GLRenderer.h"

#include <glad/glad.h>

using namespace GL;

void GL::GLRenderer::clear()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void GL::GLRenderer::draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader)
{
	va.bind();
	shader.bind();
	glDrawElements(GL_TRIANGLES, ib.getCount(), GL_UNSIGNED_INT, 0);
}

void GL::GLRenderer::draw(const VertexArray& va, int count, const Shader& shader)
{
	va.bind();
	shader.bind();
	glDrawArrays(GL_TRIANGLES, 0, count);
}
