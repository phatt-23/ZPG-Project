//
// Created by phatt on 10/23/25.
//

#ifndef WORKSPACE_LIGHT_STRUCT_H
#define WORKSPACE_LIGHT_STRUCT_H

namespace ZPG {

// Structure mirroring the light struct in used in shader programs.
// Must match std140 layout rules from GLSL
struct alignas(16) LightStruct {
    i32 Type        = 0;
    f32 _pad0[3];

    v4 Color        = v4(0.0);
    v3 Pos          = v3(0.0);
    f32 _pad1;

    v3 Dir          = v3(0.0, 0.0, -1.0);
    f32 _pad2;

    f32 BeamSize    = 0.5;
    f32 BeamBlend   = 30;
    f32 _pad3[2];
};

}

#endif //WORKSPACE_LIGHT_STRUCT_H