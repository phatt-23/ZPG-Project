//
// Created by phatt on 11/14/25.
//

#ifndef WORKSPACE_DIRECTIONALLIGHT_H
#define WORKSPACE_DIRECTIONALLIGHT_H

struct DirectionalLight
{
    vec4 Color;
    vec3 Direction;
    mat4 ViewProj;
};

#endif //WORKSPACE_DIRECTIONALLIGHT_H
