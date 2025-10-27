#pragma once

namespace ZPG {

class Mesh;

class Model {
public:
    Model(const std::vector<ref<Mesh>>& meshes);
    ~Model();

    const std::string& GetName() const;
    const std::vector<ref<Mesh>>& GetMeshes() const;

    void SetName(std::string const& name);

    static ref<Model> Create(const std::vector<ref<Mesh>>& meshes);
    static ref<Model> Create(const std::string& name, const std::vector<ref<Mesh>>& meshes);
private:
    std::vector<ref<Mesh>> m_Meshes;
    std::string m_Name;
};

}
