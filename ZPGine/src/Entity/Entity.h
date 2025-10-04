#pragma once
#include "Core/Core.h"
#include "Model/Mesh.h"
#include "Transform/Transform.h"
#include "Model/Model.h"

namespace ZPG {

class Entity {
public:
    Entity(const Ref<Model>& model, const Ref<Transform>& transform) 
    : m_Model(model)
    , m_Transform(transform) {

    }

    virtual ~Entity() {}

    void Update(Timestep ts) {
        m_Transform->Update(ts);
    }

    const glm::mat4& GetTransformMatrix() {
        return m_Transform->GetMatrix();
    }

    const Ref<Model>& GetModel() const { 
        return m_Model; 
    }

private:
    Ref<Model> m_Model;
    Ref<Transform> m_Transform;
};

}