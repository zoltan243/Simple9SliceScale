#include "my_window.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include <exception>
#include <string>

MyWindow::MyWindow()
{
}

MyWindow::~MyWindow()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(m_window);
    glfwTerminate();
}

void MyWindow::init(int width, int height)
{
    if (!glfwInit())
        throw std::exception("Can't initialise GLFW!");

    const char* glsl_version = "#version 330";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
    glfwWindowHint(GLFW_SAMPLES, 4);

    m_window = glfwCreateWindow(width, height, "9-slice with ImGui", nullptr, nullptr);
    if (!m_window)
        throw std::exception("Can't create window!");

    glfwMakeContextCurrent(m_window);
    glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    glfwSetKeyCallback(m_window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
        {
            if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
                glfwSetWindowShouldClose(window, GLFW_TRUE);
        });

    unsigned int status = glewInit();
    if (status != GLEW_OK)
        throw std::exception("Can't initialise GLEW!");

    glViewport(0, 0, width, height);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(m_window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
}

bool MyWindow::shouldClose()
{
    return glfwWindowShouldClose(m_window);
}

glm::vec2 MyWindow::createImGuiWindow(unsigned int textureID)
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImVec2 windowSize;

    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.9f, 0.9f, 0.9f, 1.0f));
    ImGui::Begin("Very Ugly Example", (bool*)0);
    {
        ImGuiStyle& style = ImGui::GetStyle();
        style.WindowPadding = ImVec2(0, 0);
        style.WindowRounding = 0;
        style.WindowBorderSize = 0;
        style.WindowMinSize = ImVec2(250, 200);

        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.0, 0.0, 0.0, 1.0));

        ImGui::BeginChild("Background");
        {
            windowSize = ImGui::GetWindowSize();

            ImGui::Image((void*)textureID, windowSize);

            ImGui::SetCursorPos(ImVec2(30, 30));
            ImGui::Text("Some input");
            ImGui::SameLine(0.0f, ImGui::CalcTextSize("Some input").x - 50);
            ImGui::SetNextItemWidth(windowSize.x - ImGui::GetCursorPosX() - 30);
            static std::string textHolder;
            textHolder.resize(100);
            ImGui::InputText("##something", (char*)textHolder.c_str(), textHolder.size(), ImGuiInputTextFlags_AlwaysInsertMode);

            ImGui::SetCursorPos(ImVec2(30, windowSize.y - 50));
            ImGui::Button("Button Left");

            ImGui::SetCursorPos(ImVec2(windowSize.x - ImGui::CalcTextSize("Button Right").x - 40, windowSize.y - 50));
            ImGui::Button("Button Right");

            ImGui::PopStyleColor(1);
        }
        ImGui::EndChild();
    }
    ImGui::End();
    ImGui::PopStyleColor(1);

    return glm::vec2(windowSize.x, windowSize.y);
}

void MyWindow::update()
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwPollEvents();
    glfwSwapBuffers(m_window);
}

void MyWindow::clear()
{
    glClearColor(0.05f, 0.2f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}
