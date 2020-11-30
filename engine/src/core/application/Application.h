#pragma once

/**
  * Created by Joscha Vack on 2/20/2020.
  *
  **/

#include <string>
#include <core/events/MouseEvent.h>

#include "core/layerStack/LayerStack.h"
#include "core/imGuiLayer/ImGuiLayer.h"
#include "core/window/Window.h"
#include "core/events/Event.h"
#include "core/events/WindowEvent.h"

namespace Engine {

    struct ApplicationProps {
        std::string name;
        WindowProps windowProps;

        ApplicationProps(const std::string& name = "App", WindowProps windowProps = WindowProps()) : name(name), windowProps(windowProps) {}
    };

    class Application {
    public:
        explicit Application(const ApplicationProps& props);
        virtual ~Application() = default;

        void run();
        void stop();

        void onEvent(Event& e);

        void pushLayer(Layer* layer);
        void popLayer(Layer* layer);

        void pushOverlay(Layer* overlay);
        void popOverlay(Layer* overlay);

        inline Window& getWindow() { return *window; }
        inline static Application& get() { return *instance; };

        inline bool isRunning() { return running; }
        inline bool isFocused() { return focused; }

    protected:
        std::string name;

    private:
        bool onWindowResizeEvent(WindowResizeEvent& e);
        bool onWindowFocusEvent(WindowFocusEvent& e);
        bool onWindowCloseEvent(WindowCloseEvent& e);

        std::unique_ptr<Window> window;
        LayerStack layerStack;
        bool running, focused;
        float lastFrameTime = 0.0f;
        ImGuiLayer* imGuiLayer;

        static Application* instance;
    };
}