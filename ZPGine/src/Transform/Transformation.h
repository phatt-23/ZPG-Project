#pragma once
#include <glm/glm.hpp>

namespace ZPG {

class Transform {
public:
    virtual ~Transform() = default;
    virtual glm::mat4 GetMatrix() = 0;
    
private:
};

}

