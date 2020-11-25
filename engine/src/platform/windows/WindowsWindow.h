#pragma once

/**
  * Created by Joscha Vack on 2/20/2020.
  *
  **/

#include <string>
#include <GLFW/glfw3.h>

#include "core/window/Window.h"

namespace Engine {

    class WindowsWindow : public Window {
    public:
        WindowsWindow(const WindowProps& props, const EventCallbackFn& callback);
        virtual ~WindowsWindow();

        void onUpdate() override;

        inline int getWidth() const override { return data.width; }
        inline int getHeight() const override { return data.height; }

        inline glm::vec2 getWindowPosition() const override {
            int x, y;
            glfwGetWindowPos(window, &x, &y);
            return glm::vec2(x, y);
        }

        // Window attributes
        void setVsync(bool enabled) override;
        bool isVsync() const override;

        inline virtual void* getNativeWindow() const { return window; }
    private:
        void init(const WindowProps& props, const EventCallbackFn& callback);
        void shutdown();
    private:
        GLFWwindow* window;

        struct WindowData
        {
            std::string name;
            uint32_t width, height;
            bool vsync;

            EventCallbackFn eventCallback;
        };

        WindowData data;
    };

}
