#include "VertexBuffer.h"

#include "Renderer.h"
#include <GL/glew.h>

VertexBuffer::VertexBuffer(const void* data, uint32_t size)
{
    GLCall(glGenBuffers(1, &m_RenderId));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RenderId));
    GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

VertexBuffer::~VertexBuffer()
{
    GLCall(glDeleteBuffers(1, &m_RenderId));
}

void VertexBuffer::Bind() const
{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RenderId));
}

void VertexBuffer::Unbind() const
{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}
