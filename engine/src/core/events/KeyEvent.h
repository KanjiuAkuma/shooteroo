#pragma once

#include <sstream>

#include "core/events/Event.h"
#include "core/codes/KeyCodes.h"

namespace Engine {

	class KeyEvent : public Event
	{
	public:
		inline KeyCode getKeyCode() const { return keyCode; }

		EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)
	protected:
        inline explicit KeyEvent(const KeyCode keycode) : keyCode(keycode) {}
		KeyCode keyCode;
	};

	class KeyPressedEvent : public KeyEvent
	{
	public:
        inline KeyPressedEvent(const KeyCode keycode, const uint16_t repeatCount)
			: KeyEvent(keycode), repeatCount(repeatCount) {}

        inline uint16_t getRepeatCount() const { return repeatCount; }

        inline std::string toString() const override
		{
			std::stringstream ss;
			ss << "KeyPressedEvent: " << keyCode << " (" << repeatCount << " repeats)";
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyPressed)
	private:
		uint16_t repeatCount;
	};

	class KeyReleasedEvent : public KeyEvent
	{
	public:
        inline explicit KeyReleasedEvent(const KeyCode keycode) : KeyEvent(keycode) {}

        inline std::string toString() const override
		{
			std::stringstream ss;
			ss << "KeyReleasedEvent: " << keyCode;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyReleased)
	};

	class KeyTypedEvent : public KeyEvent
	{
	public:
        inline explicit KeyTypedEvent(const KeyCode keycode) : KeyEvent(keycode) {}

        inline std::string toString() const override
		{
			std::stringstream ss;
			ss << "KeyTypedEvent: " << keyCode;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyTyped)
	};
}