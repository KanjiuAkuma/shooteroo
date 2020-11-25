/**
  * Created by Joscha Vack on 2/20/2020.
  *
  **/

#include <GLFW/glfw3.h>
#include <core/window/Input.h>

#include "Macros.h"
#include "Application.h"
#include "core/logger/Logger.h"

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
            float time = (float) glfwGetTime();
            float timestep = time - lastFrameTime;
            lastFrameTime = time;

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
        dispatcher.dispatch<WindowCloseEvent>(BIND_EVENT_FN(Application::onWindowClose));

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

    bool Application::onWindowClose(WindowCloseEvent& e)
    {
        running = false;
        return true;
    }

}