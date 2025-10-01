#include "CompoundTransform.h"

namespace ZPG {

glm::mat4 CompoundTransform::GetMatrix() {
    glm::mat4 matrix(1.f);
    for (auto& t : m_Transformations) {
        matrix = t->GetMatrix() * matrix;
    }
    return matrix;
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
