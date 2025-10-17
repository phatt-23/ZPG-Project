//
// Created by phatt on 10/17/25.
//

#ifndef SKYBOX_H
#define SKYBOX_H
#include "Core/Core.h"

namespace ZPG {

class Texture;
class VertexArray;
class ShaderProgram;

constexpr f32 skyboxVertices[] = {
    // position
    -1.0f, -1.0f,  1.0f,
	-1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f, -1.0f,
	 1.0f,  1.0f, -1.0f,
	 1.0f,  1.0f,  1.0f,
};

constexpr u32 skyboxIndices[] = {
    0, 1, 2, 0, 2, 3, // Front face
    0, 4, 7, 0, 7, 3, // Left face
    3, 7, 6, 3, 6, 2, // Right face
    2, 6, 5, 2, 5, 1, // Back face
    1, 5, 4, 1, 4, 0, // Bottom face
    4, 5, 6, 4, 6, 7, // Top face
};

class Skybox {
public:
    Skybox(const std::string& texturesDir);



public:
    ref<ShaderProgram> m_ShaderProgram;
	ref<VertexArray> m_VAO;
	ref<Texture> m_Texture;
};

} // ZPG

#endif //SKYBOX_H
