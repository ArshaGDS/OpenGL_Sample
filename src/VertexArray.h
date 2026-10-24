#pragma once

#include "VertexBufferLayout.h"

class VertexBuffer;

class VertexArray
{
public:
    VertexArray();
    ~VertexArray();

    void AddBuffer(VertexBuffer& vb, VertexBufferLayout& layout);
    void Bind() const;
    void Unbind() const;

private:
    uint32_t m_RendererID;
};
