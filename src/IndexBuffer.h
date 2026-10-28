#pragma once
#include <cstdint>

class IndexBuffer
{
public:
    IndexBuffer(const uint32_t* data, int32_t count);
    ~IndexBuffer();

    void Bind() const;
    void Unbind() const;

    [[nodiscard]] uint32_t GetCount() const { return m_Count; }

private:
    uint32_t m_RenderId{};
    uint32_t m_Count{};
};
