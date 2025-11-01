//
// Created by phatt on 10/17/25.
//

#ifndef SKYBOX_H
#define SKYBOX_H

namespace ZPG {

class Texture;
class VertexArray;
class ShaderProgram;

struct SkyboxSpecification {
    std::string Directory;
    std::array<std::string, 6> Filenames = {
        "right.jpg", "left.jpg", "top.jpg", "bottom.jpg", "front.jpg", "back.jpg"
    };
};

class Skybox {
public:
    virtual ~Skybox() = default;

    virtual void Bind() = 0;
    virtual void Unbind() = 0;

    virtual const ref<VertexArray>& GetVAO() const = 0;

    static ref<Skybox> Create(const SkyboxSpecification& spec);
};

} // ZPG

#endif //SKYBOX_H
