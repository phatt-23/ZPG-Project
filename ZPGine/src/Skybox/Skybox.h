//
// Created by phatt on 10/17/25.
//

#ifndef SKYBOX_H
#define SKYBOX_H

namespace ZPG {

class Texture;
class VertexArray;
class ShaderProgram;

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
