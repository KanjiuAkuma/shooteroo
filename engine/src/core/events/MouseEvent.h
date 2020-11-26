#pragma once

#include <sstream>

#include "core/events/Event.h"
#include "core/codes/MouseCodes.h"

namespace Engine {

    class MouseMovedEvent : public Event {
    public:
        inline MouseMovedEvent(const float x, const float y) : mouseX(x), mouseY(y) {}

        inline float getX() const { return mouseX; }

        inline float getY() const { return mouseY; }

        inline std::string toString() const override {
            std::stringstream ss;
            ss << "MouseMovedEvent: " << mouseX << ", " << mouseY;
            return ss.str();
        }

        EVENT_CLASS_TYPE(MouseMoved)

        EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)

    private:
        float mouseX, mouseY;
    };

    class MouseScrolledEvent : public Event {
    public:
        inline MouseScrolledEvent(const float xOffset, const float yOffset) : xOffset(xOffset), yOffset(yOffset) {}

        inline float getXOffset() const { return xOffset; }

        inline float getYOffset() const { return yOffset; }

        inline std::string toString() const override {
            std::stringstream ss;
            ss << "MouseScrolledEvent: " << getXOffset() << ", " << getYOffset();
            return ss.str();
        }

        EVENT_CLASS_TYPE(MouseScrolled)

        EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)

    private:
        float xOffset, yOffset;
    };

    class MouseButtonEvent : public Event {
    public:
        inline MouseCode getMouseButton() const { return button; }

        EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput | EventCategoryMouseButton)

    protected:
        inline explicit MouseButtonEvent(const MouseCode button) : button(button) {}

        MouseCode button;
    };

    class MouseButtonPressedEvent : public MouseButtonEvent {
    public:
        inline explicit MouseButtonPressedEvent(const MouseCode button) : MouseButtonEvent(button) {}

        inline std::string toString() const override {
            std::stringstream ss;
            ss << "MouseButtonPressedEvent: " << button;
            return ss.str();
        }

        EVENT_CLASS_TYPE(MouseButtonPressed)
    };

    class MouseButtonReleasedEvent : public MouseButtonEvent {
    public:
        inline explicit MouseButtonReleasedEvent(const MouseCode button) : MouseButtonEvent(button) {}

        inline std::string toString() const override {
            std::stringstream ss;
            ss << "MouseButtonReleasedEvent: " << button;
            return ss.str();
        }

        EVENT_CLASS_TYPE(MouseButtonReleased)
    };

}
