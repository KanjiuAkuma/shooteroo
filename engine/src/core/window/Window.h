#pragma once

/**
  * Created by Joscha Vack on 2/20/2020.
  *
  **/

#include <glm/glm.hpp>
#include <functional>
#include <core/events/Event.h>

namespace Engine {

    struct WindowProps {
        std::string name;
        bool vsync = true;
        int multisample = 8;
        bool fullScreen = true;
        int width = 0, height = 0;

        explicit WindowProps(const std::string& title = "Application Window",
                             bool vsync = true, int multisample = true,
                             bool fullScreen = true, int width = 0, int height = 0) :
                name(name), vsync(vsync), multisample(multisample), fullScreen(fullScreen), width(width),
                height(height) {};
    };

    class Window {
    public:
        using EventCallbackFn = std::function<void(Event & )>;
        virtual ~Window() = default;

        virtual void onUpdate() = 0;

        // window info
        virtual int getPositionX() const = 0;
        virtual int getPositionY() const = 0;
        virtual int getWidth() const = 0;
        virtual int getHeight() const = 0;

        virtual glm::vec2 getWindowPosition() const {
            return glm::vec2(getPositionX(), getPositionY());
        };
        virtual inline glm::vec2 getWindowSize() const {
            return glm::vec2(getWidth(), getHeight());
        }
        virtual inline glm::vec2 getWindowRatio() const {
            return glm::vec2(1.f, ((float) getHeight() / (float) getWidth()));
        }

        // viewport info
        virtual int getViewportWidth() const = 0;
        virtual int getViewportHeight() const = 0;
        virtual inline glm::vec2 getViewportSize() const {
            return glm::vec2(getViewportWidth(), getViewportHeight());
        }

        virtual inline glm::vec2 getViewportRatio() const {
            return glm::vec2(1.f, ((float) getViewportHeight() / (float) getViewportWidth()));
        }


        virtual void setVsync(bool enable) = 0;
        virtual bool isVsync() const = 0;

        virtual void* getNativeWindow() const = 0;

        static Window* create(const WindowProps& props, const EventCallbackFn& callback);
    };

};