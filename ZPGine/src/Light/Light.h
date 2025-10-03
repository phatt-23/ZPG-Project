#pragma once
#include "Core/Core.h"
#include "LightType.h"

namespace ZPG {

class Light {
public:
    u32 GetIndex() const { return m_Index; }
private:
    u32 m_Index;  
    LightType m_LightType;
};

}
