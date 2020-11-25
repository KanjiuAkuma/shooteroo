#pragma once

/**
  * Created by Joscha Vack on 2/20/2020.
  *
  **/

#include <vector>

#include "core/layer/Layer.h"

namespace Engine {

    class LayerStack {
    public:
        LayerStack();
        ~LayerStack();

        void pushLayer(Layer* layer);
        void popLayer(Layer* layer);
        void pushOverlay(Layer* overlay);
        void popOverlay(Layer* overlay);

        inline std::vector<Layer*>::iterator begin() { return layers.begin(); }
        inline std::vector<Layer*>::iterator end() { return layers.end(); }
        std::vector<Layer*>::const_reverse_iterator rbegin() const { return layers.rbegin(); }
        std::vector<Layer*>::const_reverse_iterator rend() const { return layers.rend(); }

    private:
        int layerInsertIndex = 0;
        std::vector<Layer*> layers;
    };

}
