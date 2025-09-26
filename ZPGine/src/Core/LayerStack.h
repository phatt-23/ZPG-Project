//
// Created by phatt on 9/22/25.
//

#ifndef LAYERSTACK_H
#define LAYERSTACK_H

#include "Layer.h"
#include "Core.h"

namespace ZPG {

class LayerStack {
public:
    void PushLayer(Layer* layer);
    void PushLayer(Ref<Layer> layer);
    Ref<Layer> PopLayer();

    std::vector<Ref<Layer>>::iterator begin() { return m_Layers.begin(); }
    std::vector<Ref<Layer>>::iterator end() { return m_Layers.end(); }
private:
    std::vector<Ref<Layer>> m_Layers;
};

}

#endif //LAYERSTACK_H
