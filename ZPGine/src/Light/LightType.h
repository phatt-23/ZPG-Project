#ifndef INCLUDE_LIGHT_LIGHT_TYPE_H_
#define INCLUDE_LIGHT_LIGHT_TYPE_H_

#include "Core/Core.h"

namespace ZPG {

enum class LightType : i32 {
    None        = 0,
    Ambient     = ZPG_BIT(0),
    Point       = ZPG_BIT(1),
    Directional = ZPG_BIT(2),
    Spotlight   = ZPG_BIT(3),
};

}
#endif
