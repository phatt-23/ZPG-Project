//
// Created by phatt on 10/23/25.
//

#ifndef WORKSPACE_LIGHT_STRUCT_H
#define WORKSPACE_LIGHT_STRUCT_H

namespace ZPG {
enum class LightType;
}

namespace ZPG {

// Structure mirroring the light struct in used in shader programs.

struct alignas(16) LightStruct {
    v3 Pos;  // 12
    i32 Type;  // 4

    v4 Color;  // 16

    v3 Dir;  // 12
    f32 BeamSize; // 4

    v3 Atten;  // 12
    f32 BeamBlend;  // 4
};

static_assert(sizeof(LightStruct) == 64);

/*

    Offset | Size | Member
    0 | 4 | i32 Type
    4 | 16 | v4 Color
    20 | 12 | v3 Pos
    32 | 12 | v3 Dir
    44 | 4 | f32 BeamSize
    48 | 4 | f32 BeamBlend
    Total: 52 bytes

    I need to pad it to multiple of 16, because the largest member is vec4, which has 16 bytes.
    The next multiple of 16 that's larger than 52 is 56.

 */


}

#endif //WORKSPACE_LIGHT_STRUCT_H