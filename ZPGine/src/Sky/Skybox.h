//
// Created by phatt on 10/17/25.
//

#ifndef SKYBOX_H
#define SKYBOX_H
#include "Sky.h"

namespace ZPG {

    class Texture2D;
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

    class Skybox : public Sky
    {
    public:
        virtual ~Skybox() override;

        virtual SkyType GetSkyType() const override;

        static ref<Skybox> Create(const SkyboxSpecification& spec);
    };

} // ZPG

#endif //SKYBOX_H
