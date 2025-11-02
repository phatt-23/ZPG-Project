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
        "right.jpg",    // px
        "left.jpg",     // nx
        "top.jpg",      // py
        "bottom.jpg",   // ny
        "front.jpg",    // pz
        "back.jpg"      // nz
    };
};

class Skybox {
public:
    virtual ~Skybox() = default;

    virtual void Bind() = 0;
    virtual void Unbind() = 0;

    virtual const ref<VertexArray>& GetVertexArray() const = 0;

    virtual void BindCubemapToSlot(int slot) const = 0;
    virtual void UnbindCubemap() const = 0;

    static ref<Skybox> Create(const SkyboxSpecification& spec);
};

} // ZPG

#endif //SKYBOX_H
