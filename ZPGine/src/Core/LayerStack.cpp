#include "LayerStack.h"
#include "Debug/Logger.h"

namespace ZPG {

void LayerStack::PushLayer(Layer* layer) {
    ZPG_CORE_DEBUG("Pushed layer");
    m_Layers.push_back(CreateRef<Layer>(layer));
    layer->OnAttach();
}

void LayerStack::PushLayer(Ref<Layer> layer) {
    m_Layers.push_back(layer);
    layer->OnAttach();
}

Ref<Layer> LayerStack::PopLayer() {
    Ref<Layer> lastLayer = m_Layers.back();
    m_Layers.pop_back();
    lastLayer->OnDetach();
    return lastLayer;
}

}

