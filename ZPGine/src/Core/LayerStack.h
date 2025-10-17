//
// Created by phatt on 9/22/25.
//

#ifndef LAYERSTACK_H
#define LAYERSTACK_H

namespace ZPG {

class Layer;

class LayerStack {
public:
    void PushLayer(Layer* layer);
    void PushLayer(ref<Layer> layer);
    ref<Layer> PopLayer();

    std::vector<ref<Layer>>::iterator begin() { return m_Layers.begin(); }
    std::vector<ref<Layer>>::iterator end() { return m_Layers.end(); }
private:
    std::vector<ref<Layer>> m_Layers;
};

}

#endif //LAYERSTACK_H
