#pragma once
#include "Model/Model.h"
#include "ModelLoader.h"
#include <unordered_map>

namespace ZPG {

class ModelLibrary {
public:
    ModelLibrary();
    ~ModelLibrary();

    void AddModel(const std::string& name, const Ref<Model>& model);
    const Ref<Model>& LoadModel(const std::string& name, const std::string& path);
    bool Exists(const std::string& name) const;
    const Ref<Model>& GetModel(const std::string& name) const;
private:
    std::unordered_map<std::string, Ref<Model>> m_ModelMap;
};

}
