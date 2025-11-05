#pragma once

#include "Core/Core.h"

namespace ZPG
{
    class VertexArray;


    enum class SkyType {
        None = 0,
        Skybox = ZPG_BIT(0),
        Skydome = ZPG_BIT(1),
    };

    class Sky
    {
    public:
        virtual ~Sky() = default;

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;
        virtual const ref<VertexArray>& GetVertexArray() const = 0;
        virtual void BindTextureToSlot(int slot) const = 0;

        virtual SkyType GetSkyType() const = 0;
    };
}
