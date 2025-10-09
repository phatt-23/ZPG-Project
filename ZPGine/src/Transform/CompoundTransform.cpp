#include "CompoundTransform.h"

namespace ZPG {


Ref<CompoundTransform> CompoundTransform::Create() {
    return CreateRef<CompoundTransform>();
}

CompoundTransform::CompoundTransform() 
: m_Transformations() {
}

CompoundTransform::CompoundTransform(CompoundTransform& other) {
    this->m_Transformations = other.m_Transformations;
}

const glm::mat4& CompoundTransform::GetMatrix() {
    ComputeMatrix();
    return m_Matrix;
}
void CompoundTransform::Update(Timestep ts) {
    for (auto& t : m_Transformations) {
        t->Update(ts);
    }
}
void CompoundTransform::ComputeMatrix() {
    m_Matrix = glm::mat4(1.0f);
    for (auto& t : m_Transformations) {
        m_Matrix = t->GetMatrix() * m_Matrix;
    }
}

void CompoundTransform::Push(const Ref<Transform>& transformation) {
    m_Transformations.push_back(std::move(transformation));
}

Ref<Transform> CompoundTransform::Pop() {
    auto& tr = m_Transformations.back();
    m_Transformations.pop_back();
    return std::move(tr);
}

}
