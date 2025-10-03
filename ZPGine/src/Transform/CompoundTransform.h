#pragma once
#include <vector>
#include "Transformation.h"
#include "Core/Core.h"

namespace ZPG {

class CompoundTransform : public Transform {
public:
    CompoundTransform() : m_Transformations() {
    }
    CompoundTransform(CompoundTransform& other) {
        this->m_Transformations = other.m_Transformations;
    }

    glm::mat4 GetMatrix() override;

    void Push(const Ref<Transform>& transformation);  // there has to be indirection, because it's an abstract class

    template<typename T, typename... Args>
    requires(std::is_base_of_v<Transform, T>)  // T derives form Transformation
    void Emplace(Args&& ...args) {
        m_Transformations.push_back(CreateRef<T>(std::forward<Args>(args)...));
    }

    Ref<Transform> Pop();
private:
    std::vector<Ref<Transform>> m_Transformations;
};

}
