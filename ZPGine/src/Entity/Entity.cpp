//
// Created by phatt on 10/17/25.
//
#include "Entity.h"
#include "Model/Model.h"
#include "Profiling/Instrumentor.h"
#include "Transform/Transform.h"

namespace ZPG {

/**
 * Counter of how many times the entity's constructor has been called.
 */
static u32 s_EntityCount = 1;

Entity::Entity(const ref<Model>& model, const ref<Transform>& transform)
: m_EntityID(s_EntityCount++)
, m_Model(model)
, m_Transform(transform)
{
    ZPG_PROFILE_FUNCTION();
    m_Transform->ComputeMatrix();
}

Entity::~Entity() {

}

void Entity::Update(Timestep& ts) {
    ZPG_PROFILE_FUNCTION();
    m_Transform->Update(ts);
}

const glm::mat4& Entity::GetTransformMatrix() const {
    ZPG_PROFILE_FUNCTION();
    return m_Transform->GetMatrix();
}

const ref<Transform>& Entity::GetTransform() const {
    ZPG_PROFILE_FUNCTION();
    return m_Transform;
}

const ref<Model>& Entity::GetModel() const {
    ZPG_PROFILE_FUNCTION();
    return m_Model;
}

i32 Entity::GetEntityID() const {
    ZPG_PROFILE_FUNCTION();
    return m_EntityID;
}

}
