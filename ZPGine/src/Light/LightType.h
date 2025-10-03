#ifndef INCLUDE_LIGHT_LIGHTTYPE_H_
#define INCLUDE_LIGHT_LIGHTTYPE_H_

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
#endif  // INCLUDE_LIGHT_LIGHTTYPE_H_
