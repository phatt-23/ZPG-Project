#pragma once

#include "Transform.h"

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
    using Self = TransformGroup&;
public:

    TransformGroup(const ref<Transform>& parent = nullptr);
    TransformGroup(TransformGroup& other);

    void Update(Timestep& ts) override;
    const glm::mat4& GetMatrix() override;
    void ComputeMatrix() override;

    template<typename T, typename... Args>
    requires(std::is_base_of_v<Transform, T>)  // T derives form Transformation
    Self Add(Args&& ...args) {
        m_Transformations.push_back(MakeRef<T>(std::forward<Args>(args)...));
        return *this;
    }
    Self Include(const ref<Transform>& transformation);
    Self WithParent(ref<Transform> parent);


    static Self Build();
    ref<TransformGroup> Compose();

    // deprecated:
    static ref<TransformGroup> Create(ref<Transform> parent = nullptr);
private:
    ref<Transform> m_Parent;
    std::vector<ref<Transform>> m_Transformations;
};

}
