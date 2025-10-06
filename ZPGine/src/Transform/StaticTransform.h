#pragma once

#include "Transform/Transform.h"
namespace ZPG {

class StaticTransform : public Transform {
public:
    StaticTransform(bool cached);
    virtual ~StaticTransform() override;
    virtual const glm::mat4& GetMatrix() override;
protected:
    bool m_Cached; 
};

}
