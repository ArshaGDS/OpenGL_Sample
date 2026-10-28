#pragma once

#include <cstdint>
#include <string>

class Texture
{
private:
    uint32_t m_RenderID{};
    std::string m_FilePath{};
    uint8_t* m_LocalBuffer = nullptr;
    int32_t m_Width{};
    int32_t m_Height{};
    int32_t m_BPP{}; // Bit Per Pixel

public:
    explicit Texture(const std::string& filePath);
    ~Texture();

    void Bind(uint32_t slot = 0) const;
    void Unbind() const;

    [[nodiscard]] int32_t GetWidth() const { return m_Width; }
    [[nodiscard]] int32_t GetHeight() const { return m_Height; }
};