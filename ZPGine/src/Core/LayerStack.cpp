#include "LayerStack.h"
#include "Layer.h"
#include "Debug/Logger.h"
#include "Profiling/Instrumentor.h"

namespace ZPG {

void LayerStack::PushLayer(Layer* layer) {
    ZPG_PROFILE_FUNCTION();
    ZPG_CORE_DEBUG("Pushing layer...");
    m_Layers.push_back(MakeRef<Layer>(layer));
    ZPG_CORE_DEBUG("Calling layer's OnAttach...");
    layer->OnAttach();
}

void LayerStack::PushLayer(ref<Layer> layer) {
    ZPG_PROFILE_FUNCTION();
    m_Layers.push_back(layer);
    layer->OnAttach();
}

ref<Layer> LayerStack::PopLayer() {
    ZPG_PROFILE_FUNCTION();
    ref<Layer> lastLayer = m_Layers.back();
    m_Layers.pop_back();
    lastLayer->OnDetach();
    return lastLayer;
}

}

