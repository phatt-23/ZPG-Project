#pragma once
#include "Core/Core.h"
#include "Model/Mesh.h"
#include "Transform/Transform.h"
#include "Model/Model.h"

namespace ZPG {

class Entity {
public:
    Entity(const ref<Model>& model, const ref<Transform>& transform) 
    : m_Model(model)
    , m_Transform(transform) {
        m_Transform->ComputeMatrix();  
    }

    virtual ~Entity() {}

    void Update(Timestep ts) {
        m_Transform->Update(ts);
    }

    const glm::mat4& GetTransformMatrix() const {
        return m_Transform->GetMatrix();
    }

    const ref<Transform>& GetTransform() const {
        return m_Transform;
    }

    const ref<Model>& GetModel() const { 
        return m_Model; 
    }

private:
    ref<Model> m_Model;
    ref<Transform> m_Transform;
};

}
