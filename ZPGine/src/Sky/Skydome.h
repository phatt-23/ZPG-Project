#pragma once

#include "Sky.h"
#include "Core/Core.h"

namespace ZPG
{
    struct SkydomeSpecification
    {
        std::string TexturePath; // path to the texture
    };

    class ShaderProgram;
    class Mesh;
    class VertexArray;
    class Texture;

    class Skydome : public Sky
    {
    public:
        virtual ~Skydome() override;

        virtual SkyType GetSkyType() const override;

        static ref<Skydome> Create(const SkydomeSpecification& spec);
    };
}
