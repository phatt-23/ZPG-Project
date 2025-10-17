#include "TransformGroup.h"

namespace ZPG {

ref<TransformGroup> TransformGroup::Create(ref<Transform> parent) {
    return MakeRef<TransformGroup>(parent);
}

TransformGroup::TransformGroup(ref<Transform> parent)
: m_Parent(parent)
, m_Transformations() {
}

TransformGroup::TransformGroup(TransformGroup& other) {
    this->m_Transformations = other.m_Transformations;
}

TransformGroup::Self TransformGroup::WithParent(ref<Transform> parent) {
    m_Parent = parent; 
    return *this; 
}

const glm::mat4& TransformGroup::GetMatrix() {
    ComputeMatrix();
    return m_Matrix;
}
void TransformGroup::Update(Timestep ts) {
    for (auto& t : m_Transformations) {
        t->Update(ts);
    }
    ComputeMatrix();
}
void TransformGroup::ComputeMatrix() {
    m4 m(1.0f);
    for (auto& t : m_Transformations) {
        m = t->GetMatrix() * m;
    }
    if (m_Parent != nullptr) {
        m = m_Parent->GetMatrix() * m;
    }
    m_Matrix = m;
}

TransformGroup::Self TransformGroup::Include(const ref<Transform>& transformation) {
    m_Transformations.push_back(std::move(transformation));
    return *this;
}

}
