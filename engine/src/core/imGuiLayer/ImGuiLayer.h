#pragma once

/**
  * Created by Joscha Vack on 2/20/2020.
  *
  **/


#include "core/layer/Layer.h"

namespace Engine {
    class ImGuiLayer : public Layer {
    public:
        ImGuiLayer();

        void OnEvent(Event& e) override;

        void onAttach() override;
        void onDetach() override;

        void begin();
        void end();
    };

}
