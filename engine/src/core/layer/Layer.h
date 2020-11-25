#pragma once

/**
  * Created by Joscha Vack on 2/20/2020.
  *
  **/

#include <string>

#include "core/events/Event.h"

namespace Engine {

    class Layer {
    public:
        explicit Layer(const std::string& name = "Layer");
        virtual ~Layer() = default;

        inline virtual void onAttach() {};
        inline virtual void onDetach() {};

        inline virtual void onUpdate(float dt) {};
        inline virtual void onImGuiRender() {};

        virtual void OnEvent(Event& event) {}

    protected:
        std::string debugName;
    };

}
