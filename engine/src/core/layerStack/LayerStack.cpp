/**
  * Created by Joscha Vack on 2/20/2020.
  *
  **/

#include "LayerStack.h"

namespace Engine {

    LayerStack::LayerStack() {}

    LayerStack::~LayerStack() {
        for (Layer* l : layers) {
            delete l;
        }
    }

    void LayerStack::pushLayer(Layer* layer) {
        layers.emplace(layers.begin() + layerInsertIndex, layer);
        layerInsertIndex++;
        layer->onAttach();
    }

    void LayerStack::popLayer(Layer* layer) {
        auto it = std::find(layers.begin(), layers.begin() + layerInsertIndex, layer);
        if (it != layers.begin() + layerInsertIndex) {
            layers.erase(it);
            layerInsertIndex--;
            layer->onDetach();
        }
    }

    void LayerStack::pushOverlay(Layer* overlay) {
        layers.push_back(overlay);
        overlay->onAttach();
    }

    void LayerStack::popOverlay(Layer* overlay) {
        auto it = std::find(layers.begin() + layerInsertIndex, layers.end(), overlay);
        if (it != layers.end()) {
            layers.erase(it);
            overlay->onDetach();
        }
    }

}