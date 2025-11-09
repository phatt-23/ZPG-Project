#include "ModelLibrary.h"

#include "ModelLoader.h"
#include "Debug/Asserter.h"
#include "Model.h"
#include "Profiling/Instrumentor.h"

namespace ZPG {

ModelLibrary::ModelLibrary() {

}
ModelLibrary::~ModelLibrary() {

}
void ModelLibrary::AddModel(const std::string& name, const ref<Model>& model) {
    ZPG_PROFILE_FUNCTION();
    ZPG_CORE_ASSERT(!Exists(name), "Model with this name already in the library: {}", name);
    m_ModelMap[name] = model;
}
const ref<Model>& ModelLibrary::LoadModel(const std::string& name, const std::string& path) {
    ZPG_PROFILE_FUNCTION();
    ZPG_CORE_ASSERT(!Exists(name), "Model with this name already in the library: {}, path: {}", name, path);
    ModelLoader loader = ModelLoader(path);
    ref<Model> model = loader.Load();
    model->SetName(name);
    m_ModelMap[name] = std::move(model);
    return m_ModelMap[name];
}
bool ModelLibrary::Exists(const std::string& name) const {
    ZPG_PROFILE_FUNCTION();
    return m_ModelMap.contains(name);
}
const ref<Model>& ModelLibrary::GetModel(const std::string& name) const {
    ZPG_PROFILE_FUNCTION();
    ZPG_CORE_ASSERT(Exists(name), "The model with this name doesn't exist: {}", name);
    return m_ModelMap.at(name);
}
const std::unordered_map<std::string, ref<Model>>& ModelLibrary::GetModels() const {
    ZPG_PROFILE_FUNCTION();
    return m_ModelMap;
}

}
