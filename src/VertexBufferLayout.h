#pragma once
#include <cstdint>
#include <type_traits>
#include <vector>

#include "Renderer.h"
#include "GL/glew.h"

struct VertexBufferElement
{
    uint32_t type;
    uint32_t count;
    uint8_t normalized;

    static uint32_t GetSizeOfType(uint32_t type)
    {
        switch (type)
        {
        case GL_FLOAT:         return 4;
        case GL_UNSIGNED_INT:  return 4;
        case GL_UNSIGNED_BYTE: return 1;
        }
        ASSERT(false);
        return 0;
    }
};

class VertexBufferLayout
{
public:
    VertexBufferLayout() : m_Stride(0)
    {
    }

    template <typename T>
    void Push(const uint32_t count)
    {
        uint32_t type = 0;
        uint8_t normalized = GL_FALSE;

        if constexpr (std::is_same_v<T, float>)
        {
            type = GL_FLOAT;
        }
        else if constexpr (std::is_same_v<T, uint32_t>)
        {
            type = GL_UNSIGNED_INT;
        }
        else if constexpr (std::is_same_v<T, unsigned char>)
        {
            type = GL_UNSIGNED_BYTE;
            normalized = GL_TRUE;
        }
        else
        {
            static_assert(std::is_same_v<T, void>, "Unsupported vertex attribute type");
        }

        m_Elements.push_back(
            {
                .type = type,
                .count = count,
                .normalized = normalized
            }
        );
        m_Stride += count * VertexBufferElement::GetSizeOfType(type);
    }

    [[nodiscard]] const std::vector<VertexBufferElement>& GetElements() const
    {
        return m_Elements;
    }

    [[nodiscard]] uint32_t GetStride() const
    {
        return m_Stride;
    }

private:
    std::vector<VertexBufferElement> m_Elements;
    uint32_t m_Stride;
};
