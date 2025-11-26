#include "TransformGroup.h"

#include "Debug/Asserter.h"
#include "Profiling/Instrumentor.h"

namespace ZPG 
{

    TransformGroup::TransformGroup(const ref<Transform>& parent)
        : m_Parent(parent)
        , m_Transformations()
    {
        ZPG_PROFILE_FUNCTION();
    }

    TransformGroup::TransformGroup(TransformGroup& other)
    {
        ZPG_PROFILE_FUNCTION();
        this->m_Transformations = other.m_Transformations;
    }

    TransformGroup& TransformGroup::WithParent(const ref<Transform>& parent, TransformChain chain)
    {
        ZPG_PROFILE_FUNCTION();
        m_ParentChain = chain;
        m_Parent = parent;
        return *this; 
    }

    const glm::mat4& TransformGroup::GetMatrix()
    {
        ZPG_PROFILE_FUNCTION();
        return m_Matrix;
    }

    void TransformGroup::Update(Timestep& ts)
    {
        ZPG_PROFILE_FUNCTION();
        for (auto& t : m_Transformations)
        {
            t->Update(ts);
        }
        ComputeMatrix();
    }
    void TransformGroup::ComputeMatrix()
    {
        ZPG_PROFILE_FUNCTION();

        m4 m(1.0f);
        for (auto& t : m_Transformations)
        {
            m = t->GetMatrix() * m;
        }

        if (m_Parent != nullptr)
        {
            const m4& parent = m_Parent->GetMatrix();

            switch (m_ParentChain)
            {
                case TransformChainPositionOnly:
                {
                    v3 position = m_Parent->GetPositionVector();
                    m = glm::translate(m4(1.0f), position) * m;
                } break;
                case TransformChainComplete:
                {
                    m = parent * m;
                } break;
                case TransformChainNone:
                default:
                    ZPG_UNREACHABLE("Not supported.");
                    break;
            }
        }

        m_Matrix = m;
    }

    TransformGroup& TransformGroup::Include(const ref<Transform>& transformation)
    {
        ZPG_PROFILE_FUNCTION();
        m_Transformations.push_back(std::move(transformation));
        return *this;
    }


    TransformGroup& TransformGroup::Build()
    {
        ZPG_PROFILE_FUNCTION();
        return *(new TransformGroup());
    }

    ref<TransformGroup> TransformGroup::Compose() {
        ZPG_PROFILE_FUNCTION();
        this->ComputeMatrix();
        return MakeRef(this);
    }

    const std::vector<ref<Transform>>& TransformGroup::GetChildren() const {
        ZPG_PROFILE_FUNCTION();
        return m_Transformations;
    }

}
