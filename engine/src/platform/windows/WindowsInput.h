#pragma once

/**
  * Created by Joscha Vack on 2/20/2020.
  *
  **/

#include "core/window/Input.h"

namespace Engine {

    class WindowsInput : public Input {

    protected:
        virtual glm::vec2 getMousePositionImpl() override;

        virtual float getMouseXImpl() override;
        virtual float getMouseYImpl() override;

        bool IsLeftMouseButtonDownImp() override;
        bool IsRightMouseButtonDownImp() override;

    };

}
