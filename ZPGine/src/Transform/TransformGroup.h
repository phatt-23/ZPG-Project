#pragma once
#include <vector>
#include "Transform.h"
#include "Core/Core.h"

namespace ZPG {

/**
 *  Hierarchical transformation compound element.
 * Can have a parent, any of its transforms will be local to the parent.
 * This is achieved by multiplying the parent matrix at the end of ComputeMatrix.
 * 
 * 
 * Add<T>()   - adds a transform layer 
 * Include()  - composes another compound transform 
 * Parent()   - sets hierarchical parent 
 * Compose()  - finalizes the transform
 */
class TransformGroup : public Transform {
public:
    using Self = TransformGroup&;

    TransformGroup(ref<Transform> parent = nullptr);
    TransformGroup(TransformGroup& other);

    void Update(Timestep ts) override;
    const glm::mat4& GetMatrix() override;

    Self WithParent(ref<Transform> parent);
    Self Include(const ref<Transform>& transformation);  

    template<typename T, typename... Args>
    requires(std::is_base_of_v<Transform, T>)  // T derives form Transformation
    Self Add(Args&& ...args) {
        m_Transformations.push_back(MakeRef<T>(std::forward<Args>(args)...));
        return *this;
    }

    void ComputeMatrix() override;

    static ref<TransformGroup> Create(ref<Transform> parent = nullptr);

    static Self Build() { return *(new TransformGroup()); }
    ref<TransformGroup> Compose() { return MakeRef(this); }

private:
    ref<Transform> m_Parent;
    std::vector<ref<Transform>> m_Transformations;
};

}
