#pragma once
#include <filesystem>
#include <string>
#include <unordered_map>

struct ShaderProgramSources
{
    std::string VertexSource;
    std::string FragmentSource;
};

enum class ShaderType: int8_t
{
    NONE = -1,
    VERTEX = 0,
    FRAGMENT = 1
};

class Shader
{
public:
    explicit Shader(const std::string& filepath);
    ~Shader();

    void Bind() const;
    void Unbind() const;

    // Set uniform
    void SetUniform1f(const std::string& name, float value);
    void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);

private:
    uint32_t m_RendererID;
    std::string m_FilePath;
    std::unordered_map<std::string, int32_t> m_UniformLocationCache;

private:
    uint32_t GetUniformLocation(const std::string& name);
    ShaderProgramSources ParseShader(const std::filesystem::path& filePath);
    uint32_t CompileShader(unsigned int type, const std::string& source);
    uint32_t CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
};
