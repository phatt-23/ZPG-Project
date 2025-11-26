#pragma once

#include "Transform.h"

namespace ZPG 
{
    enum TransformChain : u32
    {
        TransformChainNone = 0,
        TransformChainPositionOnly = ZPG_BIT(1),
        TransformChainComplete = ZPG_BIT(2),
        TransformChainDefault = TransformChainPositionOnly,
    };

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
    class TransformGroup : public Transform 
    {
    public:
        TransformGroup(const ref<Transform>& parent = nullptr);
        TransformGroup(TransformGroup& other);

        void Update(Timestep& ts) override;
        void ComputeMatrix() override;

        template<typename T, typename... Args>
        requires(std::is_base_of_v<Transform, T>)  
        TransformGroup& Add(Args&& ...args)
        {
            m_Transformations.push_back(MakeRef<T>(std::forward<Args>(args)...));
            return *this;
        }

        TransformGroup& Include(const ref<Transform>& transformation);

        TransformGroup& WithParent(const ref<Transform>& parent, TransformChain chainFlags = TransformChainDefault);

        const m4& GetMatrix() override;
        const std::vector<ref<Transform>>& GetChildren() const;

        static TransformGroup& Build();
        ref<TransformGroup> Compose();

    private:
        ref<Transform> m_Parent;
        TransformChain m_ParentChain = TransformChainDefault;

        std::vector<ref<Transform>> m_Transformations;
    };

}
