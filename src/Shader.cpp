#include "Shader.h"

#include <string>
#include <fstream>
#include <filesystem>
#include <sstream>
#include <iostream>

#include "File.h"
#include "Renderer.h"
#include "GL/glew.h"
#include "gtc/type_ptr.hpp"

Shader::Shader(const std::string& filepath): m_FilePath(filepath)
{
    const ShaderProgramSources shaderSources = ParseShader(File::FindResource(m_FilePath));
    m_RendererID = CreateShader(shaderSources.VertexSource, shaderSources.FragmentSource);
}

Shader::~Shader()
{
    GLCall(glDeleteProgram(0));
}

void Shader::Bind() const
{
    GLCall(glUseProgram(m_RendererID));
}

void Shader::Unbind() const
{
    GLCall(glUseProgram(0));
}

void Shader::SetUniform1i(const std::string& name, int32_t value)
{
    GLCall( glUniform1i( GetUniformLocation(name), value) );
}

void Shader::SetUniform1f(const std::string& name, float value)
{
    GLCall( glUniform1f( GetUniformLocation(name), value) );
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
    GLCall( glUniform4f( GetUniformLocation(name), v0, v1, v2, v3 ) );
}

void Shader::SetUniformMat4(const std::string& name, const glm::mat4& matrix)
{
    glUniformMatrix4fv( static_cast<int32_t>(GetUniformLocation(name)), 1, GL_FALSE, glm::value_ptr(matrix) );
}

uint32_t Shader::GetUniformLocation(const std::string& name)
{
    if (m_UniformLocationCache.contains(name))
        return m_UniformLocationCache[name];

    GLCall(const int32_t location = glGetUniformLocation(m_RendererID, name.c_str()));
    if (location == -1)
    {
        std::cout << "Warning: uniform " << name << " doesn't exist!" << std::endl;
    }

    m_UniformLocationCache[name] = location;
    return location;
}

ShaderProgramSources Shader::ParseShader(const std::filesystem::path& filePath)
{
    std::ifstream ifs(filePath);
    if (!ifs.is_open())
    {
        std::cerr << "Failed to open shader file: " << filePath << "\n";
        std::cerr << "Reason: " << std::strerror(errno) << "\n";
        std::cerr << "Current working directory: " << std::filesystem::current_path() << std::endl;
        return {};
    }

    std::string line;
    std::stringstream sstr[2];
    auto shaderType = ShaderType::NONE;

    while (std::getline(ifs, line))
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
            {
                shaderType = ShaderType::VERTEX;
            }
            else if (line.find("fragment") != std::string::npos)
            {
                shaderType = ShaderType::FRAGMENT;
            }
        }
        else if (shaderType != ShaderType::NONE) // Prevent writing to sstr[-1]
        {
            sstr[(int)shaderType] << line << '\n';
        }
    }

    return {
        .VertexSource = sstr[0].str(),
        .FragmentSource = sstr[1].str()
    };
}

uint32_t Shader::CompileShader(const unsigned int type, const std::string & source)
{
    const uint32_t id = glCreateShader(type);
    const char* src = source.c_str();

    GLCall(glShaderSource(id, 1, &src, nullptr));
    GLCall(glCompileShader(id));

    int result;
    GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
    if (result == GL_FALSE)
    {
        int length;
        GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
        auto message = std::string(length, '\0');
        GLCall(glGetShaderInfoLog(id, length, &length, &message[0]));
        std::cout << "Failed to compile shader" << std::endl;
        std::cout << message << std::endl;
    }

    return id;
}

uint32_t Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    const uint32_t program = glCreateProgram();
    const uint32_t vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    const uint32_t fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    GLCall(glAttachShader(program, vs));
    GLCall(glAttachShader(program, fs));
    GLCall(glLinkProgram(program));
    GLCall(glValidateProgram(program));

    GLCall(glDeleteShader(vs));
    GLCall(glDeleteShader(fs));

    return program;
}