/**
  * Created by Joscha Vack on 2/20/2020.
  *
  **/

#include <glfw/glfw3.h>
#include <imgui.h>

#include "WindowsInput.h"
#include "core/application/Application.h"

namespace Engine {

    Input* Input::instance = new WindowsInput();

    glm::vec2 WindowsInput::getMousePositionImpl() {
        auto window = static_cast<GLFWwindow*>(Application::get().getWindow().getNativeWindow());
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);

        return glm::vec2(
                -1.f + 2.f * ((float) xpos) / ((float) Application::get().getWindow().getWidth()),
                1.f - 2.f * ((float) ypos) / ((float) Application::get().getWindow().getHeight()));
    }

    float WindowsInput::getMouseXImpl() {
        auto window = static_cast<GLFWwindow*>(Application::get().getWindow().getNativeWindow());
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);

        return -1.f + 2.f * ((float) xpos) / ((float) Application::get().getWindow().getWidth());
    }

    float WindowsInput::getMouseYImpl() {
        auto window = static_cast<GLFWwindow*>(Application::get().getWindow().getNativeWindow());
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        glm::vec2 screen = Application::get().getWindow().getViewportSize();

        return 1.f - 2.f * ((float) ypos) / ((float) Application::get().getWindow().getHeight());
    }

    bool WindowsInput::IsLeftMouseButtonDownImp() {
        return !ImGui::GetIO().WantCaptureMouse && glfwGetMouseButton(static_cast<GLFWwindow*>(Application::get().getWindow().getNativeWindow()), GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;
    }

    bool WindowsInput::IsRightMouseButtonDownImp() {
        return !ImGui::GetIO().WantCaptureMouse && glfwGetMouseButton(static_cast<GLFWwindow*>(Application::get().getWindow().getNativeWindow()), GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS;
    }

}
