#pragma once

#include <sstream>

#include "core/events/Event.h"

namespace Engine {

    class WindowMovedEvent : public Event {
    public:
        inline WindowMovedEvent(int x, int y) : x(x), y(y) {}

        inline std::string toString() const override  {
            std::stringstream ss;
            ss << "WindowMovedEvent: " << x << ", " << y;
            return ss.str();
        }

        inline int getPositionX() const { return x; }
        inline int getPositionY() const { return y; }

        EVENT_CLASS_TYPE(WindowMoved);
        EVENT_CLASS_CATEGORY(EventCategoryWindow);
    private:
        int x, y;
    };

	class WindowResizeEvent : public Event
	{
	public:
        inline WindowResizeEvent(unsigned int width, unsigned int height)
			: width(width), height(height) {}

		inline unsigned int getWidth() const { return width; }
        inline unsigned int getHeight() const { return height; }

        inline std::string toString() const override
		{
			std::stringstream ss;
			ss << "WindowResizeEvent: " << width << ", " << height;
			return ss.str();
		}

		EVENT_CLASS_TYPE(WindowResize)
		EVENT_CLASS_CATEGORY(EventCategoryWindow)
	private:
		unsigned int width, height;
	};

	class WindowCloseEvent : public Event
	{
	public:
		WindowCloseEvent() = default;

		EVENT_CLASS_TYPE(WindowClose)
		EVENT_CLASS_CATEGORY(EventCategoryWindow)
	};

	class WindowFocusEvent : public Event {
	public:
	    inline explicit WindowFocusEvent(bool focused) : focused(focused) {}

	    inline bool isFocused() const { return focused; }

	    inline std::string toString() const override {
	        std::stringstream ss;
	        ss << "WindowFocusEvent: " << focused;
	        return ss.str();
	    }

	    EVENT_CLASS_TYPE(WindowFocus);
	    EVENT_CLASS_CATEGORY(EventCategoryWindow)
	private:
	    bool focused;
	};


}