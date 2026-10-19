#include <cstring>
#include <iostream>
#include <ostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <string>
#include <fstream>
#include <filesystem>
#include <sstream>

#define ASSERT(x) if (!(x)) __debugbreak(); // Its just works in MSVC compiler and Windows OS
#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))

namespace fs = std::filesystem;

static void GLClearError()
{
    while (glGetError() != GL_NO_ERROR);
}

static bool GLLogCall(const char* function, const char* file, int line)
{
    while (const GLenum error = glGetError())
    {
        std::cerr << "[OpenGL Error] (" << error << "): " << function << " " << file << ":" << line << std::endl;
        return false;
    }
    return true;
}

struct ShaderProgramSources
{
    std::string VertexSource;
    std::string FragmentSource;
};

namespace
{
    enum class ShaderType: int8_t
    {
        NONE = -1,
        VERTEX = 0,
        FRAGMENT = 1
    };
}

static ShaderProgramSources ParseShader(const fs::path& filePath)
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
    ShaderType shaderType = ShaderType::NONE;

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

static uint32_t CompileShader(const unsigned int type, const std::string & source)
{
    const uint32_t id = glCreateShader(type);
    const char* src = source.c_str();

    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE)
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        auto message = std::string(length, '\0');
        glGetShaderInfoLog(id, length, &length, &message[0]);
        std::cout << "Failed to compile shader" << std::endl;
        std::cout << message << std::endl;
    }

    return id;
}

static uint32_t CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    const uint32_t program = glCreateProgram();
    const uint32_t vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    const uint32_t fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

static fs::path FindResource(const fs::path& relative)
{
    fs::path dir = fs::current_path();
    while (true)
    {
        fs::path candidate = dir / relative;
        if (fs::exists(candidate))
            return candidate;
        if (!dir.has_parent_path() || dir == dir.parent_path())
            return {};
        dir = dir.parent_path();
    }
}

int main()
{
    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    GLFWwindow* window = glfwCreateWindow(640, 480, "Hello World", nullptr, nullptr);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK)
        return -1;

    constexpr float positions[] = {
        -0.5f, -0.5f,
         0.5f, -0.5f,
         0.5f, 0.5f,
        -0.5f, 0.5f,
    };

    constexpr uint32_t indices[] = {
        0, 1, 2,
        2, 3, 0
    };

    uint32_t bufferId;
    glGenBuffers(1, &bufferId);
    glBindBuffer(GL_ARRAY_BUFFER, bufferId);
    glBufferData(GL_ARRAY_BUFFER, 6 * 2 * sizeof(positions), positions, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);

    uint32_t ibo; // index buffer object
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(uint32_t), indices, GL_STATIC_DRAW);


    const ShaderProgramSources shaderSources = ParseShader(FindResource("res/shaders/basic.shader"));
    const uint32_t shader = CreateShader(shaderSources.VertexSource, shaderSources.FragmentSource);
    glUseProgram(shader);

    int32_t location = glGetUniformLocation(shader, "u_Color");
    ASSERT(location != -1);

    float redChanel = 0.0f;
    float increment = 0.05;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        GLCall(glUniform4f(location, redChanel, 0.3f, 0.8f, 0.1f));
        GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr))

        if (redChanel > 1.0f)
            increment = -0.05f;
        else if (redChanel < 0.0f)
            increment = 0.05f;

        redChanel += increment;

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glDeleteProgram(shader);

    glfwTerminate();
    return 0;
}