/**
  * Created by Joscha Vack on 2/20/2020.
  *
  **/

#include <GLFW/glfw3.h>

#include "Macros.h"
#include "Application.h"
#include "core/logger/Logger.h"
#include "core/events/MouseEvent.h"

namespace Engine {

    Application* Application::instance = nullptr;

    Application::Application(const ApplicationProps& props) : name(props.name) {
        if (!instance) {
            // init core

            Logger::init(props.name);
        }

        CORE_ASSERT(!instance, "Application already exists!");
        instance = this;

        window = std::unique_ptr<Window>(Window::create(props.windowProps, BIND_EVENT_FN(Application::onEvent)));

        imGuiLayer = new ImGuiLayer();
        pushOverlay(imGuiLayer);
    }

    void Application::run() {
        running = true;
        while (running) {
            float timestep = 0;
            if (focused) {
                float time = (float) glfwGetTime();
                timestep = time - lastFrameTime;
                lastFrameTime = time;
            }
            else {
                lastFrameTime = (float) glfwGetTime();
            }

            for (Layer* l : layerStack) {
                l->onUpdate(timestep);
            }

            imGuiLayer->begin();
            for (Layer* l : layerStack) {
                l->onImGuiRender();
            }
            imGuiLayer->end();
            window->onUpdate();
        }
    }

    void Application::stop() {
        running = false;
    }

    void Application::onEvent(Event& e)
    {
        EventDispatcher dispatcher(e);
        dispatcher.dispatch<WindowCloseEvent>(BIND_EVENT_FN(Application::onWindowCloseEvent));
        dispatcher.dispatch<WindowResizeEvent>(BIND_EVENT_FN(Application::onWindowResizeEvent));
        dispatcher.dispatch<WindowFocusEvent>(BIND_EVENT_FN(Application::onWindowFocusEvent));

        for (auto it = layerStack.rbegin(); it != layerStack.rend(); ++it)
        {
            if (e.handled)
                break;
            (*it)->OnEvent(e);
        }
    }

    void Application::pushLayer(Layer* layer) {
        layerStack.pushLayer(layer);
    }

    void Application::popLayer(Layer* layer) {
        layerStack.popLayer(layer);
    }

    void Application::pushOverlay(Layer* overlay) {
        layerStack.pushOverlay(overlay);
    }

    void Application::popOverlay(Layer * overlay) {
        layerStack.popOverlay(overlay);
    }

    bool Application::onWindowResizeEvent(WindowResizeEvent& e) {
        LOG_DEBUG("Window resized: {} x {}", e.getWidth(), e.getHeight());
        bool minimized = (e.getWidth() == 0 && e.getHeight() == 0);
        if (minimized) {
            focused = false;
        }
        return false;
    }

    bool Application::onWindowFocusEvent(WindowFocusEvent& e) {
        LOG_DEBUG("Window {}", e.isFocused() ? "focused" : "lost focus");
        focused = e.isFocused();
        return false;
    }

    bool Application::onWindowCloseEvent(WindowCloseEvent& e)
    {
        focused = false;
        running = false;
        return true;
    }

}