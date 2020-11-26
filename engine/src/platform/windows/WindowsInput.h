#pragma once

/**
  * Created by Joscha Vack on 2/20/2020.
  *
  **/

#include "core/window/Input.h"

namespace Engine {

    class WindowsInput : public Input {

    protected:
        virtual glm::vec2 getMousePositionImpl() const override;

        virtual float getMouseXImpl() const override;
        virtual float getMouseYImpl() const override;

        bool IsLeftMouseButtonDownImp() const override;
        bool IsRightMouseButtonDownImp() const override;

    };

}
