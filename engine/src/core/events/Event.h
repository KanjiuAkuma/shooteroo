#pragma once
#include <functional>
#include <string>

#define BIT(x) (1u << x)

namespace Engine {

	// Events in Hazel are currently blocking, meaning when an event occurs it
	// immediately gets dispatched and must be dealt with right then an there.
	// For the future, a better strategy might be to buffer events in an event
	// bus and process them during the "event" part of the update stage.

	enum class EventType
	{
		None = 0,
		WindowClose, WindowResize, WindowFocus, WindowMoved,
		KeyPressed, KeyReleased, KeyTyped,
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
	};

	enum EventCategory
	{
		None = 0u,
		EventCategoryApplication        = BIT(0u),
		EventCategoryWindow             = BIT(1u),
		EventCategoryInput              = BIT(2u),
		EventCategoryKeyboard           = BIT(3u),
		EventCategoryMouse              = BIT(4u),
		EventCategoryMouseButton        = BIT(5u)
	};

#define EVENT_CLASS_TYPE(type) inline static EventType getStaticType() { return EventType::type; }\
								inline virtual EventType getEventType() const override { return getStaticType(); }\
								inline virtual const char* getName() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category) virtual int getCategoryFlags() const override { return category; }

	class Event
	{
	public:
		virtual ~Event() = default;

		bool handled = false;

		virtual EventType getEventType() const = 0;
		virtual const char* getName() const = 0;
		virtual int getCategoryFlags() const = 0;
		virtual std::string toString() const { return getName(); }

		inline bool isInCategory(EventCategory category)
		{
			return getCategoryFlags() & category;
		}
	};

	class EventDispatcher
	{
	public:
		EventDispatcher(Event& event): event(event) {}
		
		// F will be deduced by the compiler
		template<typename T, typename F>
		inline bool dispatch(const F& func)
		{
			if (event.getEventType() == T::getStaticType())
			{
				event.handled |= func(static_cast<T&>(event));
				return true;
			}
			return false;
		}
	private:
		Event& event;
	};

	inline std::ostream& operator<<(std::ostream& os, const Event& e)
	{
		return os << e.toString();
	}

}

