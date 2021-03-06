#pragma once

/**
  * Created by Joscha Vack on 2/20/2020.
  *
  **/

#include <utility>
#include <glm/glm.hpp>

namespace Engine {

    class Input {
    protected:
        Input() = default;

    public:
        Input(const Input&) = delete;
        Input& operator=(const Input&) = delete;

        inline static glm::vec2 getMousePosition() { return instance->getMousePositionImpl(); }
        inline static float getMouseX() { return instance->getMouseXImpl(); }
        inline static float getMouseY() { return instance->getMouseYImpl(); }
        inline static bool IsLeftMouseButtonDown() { return instance->IsLeftMouseButtonDownImp(); }
        inline static bool IsRightMouseButtonDown() { return instance->IsRightMouseButtonDownImp(); }

    protected:
        virtual glm::vec2 getMousePositionImpl() const = 0;
        virtual float getMouseXImpl() const = 0;
        virtual float getMouseYImpl() const = 0;
        virtual bool IsLeftMouseButtonDownImp() const = 0;
        virtual bool IsRightMouseButtonDownImp() const = 0;

    private:
        static Input* instance;
    };

}