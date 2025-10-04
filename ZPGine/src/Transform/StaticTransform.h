#pragma once

#include "Transform/Transform.h"
namespace ZPG {

class StaticTransform : public Transform {
public:
    StaticTransform(bool computed) : m_Cached(computed) {}
    virtual ~StaticTransform() override {}

    virtual const glm::mat4& GetMatrix() override {
        if (m_Cached) 
            return m_Matrix;
        ComputeMatrix();
        m_Cached = true;
        return m_Matrix;
    }
protected:
    bool m_Cached; 
};

}
