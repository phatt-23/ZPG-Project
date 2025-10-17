#pragma once

namespace ZPG {

class Model;

class ModelLibrary {
public:
    ModelLibrary();
    ~ModelLibrary();

    void AddModel(const std::string& name, const ref<Model>& model);
    const ref<Model>& LoadModel(const std::string& name, const std::string& path);
    bool Exists(const std::string& name) const;
    const ref<Model>& GetModel(const std::string& name) const;
private:
    std::unordered_map<std::string, ref<Model>> m_ModelMap;
};

}
