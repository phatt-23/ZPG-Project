//
// Created by phatt on 11/14/25.
//

#ifndef WORKSPACE_DIRECTIONALLIGHT_H
#define WORKSPACE_DIRECTIONALLIGHT_H

struct DirectionalLight
{
    vec4 Color;  // 16
    vec3 Direction;
    float pad0[1];  // 16

    int CascadeCount;
    float pad1[3];  // 16

    float PlaneDistance[4];  // 16

    mat4 ViewProj[4];  // max cascade count
};

#endif //WORKSPACE_DIRECTIONALLIGHT_H
