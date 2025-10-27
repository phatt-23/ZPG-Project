//
// Created by phatt on 10/17/25.
//
#include "Entity.h"
#include "Model/Model.h"
#include "Transform/Transform.h"

namespace ZPG {

Entity::Entity(const ref<Model>& model, const ref<Transform>& transform)
: m_Model(model)
, m_Transform(transform) {
    m_Transform->ComputeMatrix();
}

Entity::~Entity() {}

void Entity::Update(Timestep& ts) {
    m_Transform->Update(ts);
}

const glm::mat4& Entity::GetTransformMatrix() const {
    return m_Transform->GetMatrix();
}

const ref<Transform>& Entity::GetTransform() const {
    return m_Transform;
}

const ref<Model>& Entity::GetModel() const {
    return m_Model;
}

}
