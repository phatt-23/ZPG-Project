#pragma once

namespace ZPG {

class Mesh;

class Model {
public:
    Model(const std::vector<ref<Mesh>>& meshes);
    ~Model();

    const std::vector<ref<Mesh>>& GetMeshes() const;

    static ref<Model> Create(const std::vector<ref<Mesh>>& meshes);
private:
    std::vector<ref<Mesh>> m_Meshes;
};

}
