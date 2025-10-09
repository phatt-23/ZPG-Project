#include "ModelLibrary.h"

namespace ZPG {

ModelLibrary::ModelLibrary() : m_Loader() {

}
ModelLibrary::~ModelLibrary() {

}
const Ref<Model>& ModelLibrary::LoadModel(const std::string& name, const std::string& path) {
    ZPG_CORE_ASSERT(!Exists(name));
    Ref<Model> model = m_Loader.Load(path);
    m_ModelMap[name] = std::move(model);
    return m_ModelMap[name];
}
bool ModelLibrary::Exists(const std::string& name) const {
    return m_ModelMap.contains(name);
}
const Ref<Model>& ModelLibrary::GetModel(const std::string& name) const {
    ZPG_CORE_ASSERT(Exists(name), "The model with this name doesn't exist: {}", name);
    return m_ModelMap.at(name);
}

}
