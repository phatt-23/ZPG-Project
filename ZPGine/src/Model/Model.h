#pragma once

#include "Mesh.h"

namespace ZPG {


class Model {
public:
    Model(const std::vector<Ref<Mesh>>& meshes) 
    : m_Meshes(meshes) {
    }

    ~Model() {}

    const std::vector<Ref<Mesh>>& GetMeshes() const { return m_Meshes; }

    static Ref<Model> Create(const std::vector<Ref<Mesh>>& meshes);
private:
    std::vector<Ref<Mesh>> m_Meshes;
};

}
