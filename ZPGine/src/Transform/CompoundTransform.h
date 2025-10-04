#pragma once
#include <vector>
#include "Transform.h"
#include "Core/Core.h"

namespace ZPG {

class CompoundTransform : public Transform {
public:
    CompoundTransform();
    CompoundTransform(CompoundTransform& other);

    void Update(Timestep ts) override;
    const glm::mat4& GetMatrix() override;

    void Push(const Ref<Transform>& transformation);  
    Ref<Transform> Pop(); // there has to be indirection, because it's an abstract class

    template<typename T, typename... Args>
    requires(std::is_base_of_v<Transform, T>)  // T derives form Transformation
    void Emplace(Args&& ...args) {
        m_Transformations.push_back(CreateRef<T>(std::forward<Args>(args)...));
    }

private:
    void ComputeMatrix() override;
private:
    std::vector<Ref<Transform>> m_Transformations;
};

}
