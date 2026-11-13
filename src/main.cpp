#include <cstring>
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Texture.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"

#include <glm.hpp>
#include "glm/gtc/matrix_transform.hpp"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif

int main()
{
    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    GLFWwindow* window = glfwCreateWindow(960, 540, "OpenGL Sample", nullptr, nullptr);
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


    {
        constexpr float positions[] = {
            100.0f, 100.0f, 0.0f, 0.0f,
            200.0f, 100.0f, 1.0f, 0.0f,
            200.0f, 200.0f, 1.0f, 1.0f,
            100.0f, 200.0f, 0.0f, 1.0f,
        };

        constexpr uint32_t indices[] = {
            0, 1, 2,
            2, 3, 0
        };

        GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        uint32_t vao; // vertex array object
        GLCall(glGenVertexArrays(1, &vao));
        GLCall(glBindVertexArray(vao));

        const VertexArray vertexArray;
        const VertexBuffer vertexBuffer(positions, sizeof(positions));
        VertexBufferLayout layout;

        layout.Push<float>(2);
        layout.Push<float>(2);
        vertexArray.AddBuffer(vertexBuffer, layout);

        const IndexBuffer indexBuffer(indices, sizeof(indices));

        const glm::mat4 proj = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f,-1.0f, 1.0f);
        constexpr glm::mat4 view = glm::translate(glm::mat4( 1.0f), glm::vec3(-100.0f, 0.0f, 0.0f));


        Shader shader("res/shaders/basic.shader");
        shader.Bind();


        const Texture texture("res/textures/Roya_Logo.jpg");
        texture.Bind();
        shader.SetUniform1i("u_Texture", 0);

        vertexArray.Unbind();
        vertexBuffer.Unbind();
        indexBuffer.Unbind();
        shader.Unbind();

        Renderer renderer;

        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
        // Setup Dear ImGui style
        ImGuiStyle& style = ImGui::GetStyle();
        ImGui::StyleColorsDark();
        style.FrameRounding = 3.0f;
        style.GrabRounding = 3.0f;
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 330");

        glm::vec3 translation(200.0f, 200.0f, 0.0f);
        static float f = 0.0f;

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            renderer.Clear();

            // Start the Dear ImGui frame
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            glm::mat4 model = glm::translate(glm::mat4( 1.0f), translation);
            const glm::mat4 mvp = proj * view * model;
            shader.Bind();
            shader.SetUniformMat4("u_MVP", mvp);

            renderer.Draw(vertexArray, indexBuffer, shader);

            {
                ImGui::Begin("Debug panel");
                ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
                ImGui::SliderFloat2("Translation", &translation.x, 0.0f, 960.0f);
                ImGui::End();
            }

            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();
    return 0;
}
