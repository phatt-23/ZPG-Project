#pragma once

namespace ZPG {

class Material;

class MaterialLibrary {
public:
    void AddMaterial(const std::string& name, const ref<Material>& material);
    bool Exists(const std::string& name) const;
    const ref<Material>& GetMaterial(const std::string& name);
    const std::unordered_map<std::string, ref<Material>>& GetMaterials() const;


private:
    std::unordered_map<std::string, ref<Material>> m_Materials;
};

}
