#include "IndexBuffer.h"

#include "Renderer.h"
#include <GL/glew.h>

IndexBuffer::IndexBuffer (const uint32_t* data, const int32_t count) : m_Count(count)
{
    GLCall(glGenBuffers(1, &m_RenderId));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RenderId));
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), data, GL_STATIC_DRAW));
}

IndexBuffer::~IndexBuffer()
{
    GLCall(glDeleteBuffers(1, &m_RenderId));
}

void IndexBuffer::Bind() const
{
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RenderId));
}

void IndexBuffer::Unbind() const
{
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}
