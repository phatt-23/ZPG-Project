//
// Created by phatt on 9/22/25.
//

#ifndef TEXTURE_H
#define TEXTURE_H

#include "Texture.h"
#include "TextureDataFormat.h"

namespace ZPG {

    class Texture2D : public Texture
    {
    public:
        virtual ~Texture2D() override {}

        virtual void SetData(const void *data, u32 size) = 0;

        virtual void Resize(u32 width, u32 height) = 0;

        virtual TextureType GetTextureType() const override { return TextureType::Texture2D; }

        static ref<Texture2D> Create(
            const std::string& filepath,
            TextureDataFormat dataFormat = TextureDataFormat::RGBA8);

        static ref<Texture2D> Create(
            const std::string& name,
            const std::string& filepath,
            TextureDataFormat dataFormat = TextureDataFormat::RGBA8);                                                           
            /* There is no need for this dataFormat, it should be deduced from the loaded texture */

        static ref<Texture2D> Create(
            const std::string& name,
            u32 width,
            u32 height,
            TextureDataFormat dataFormat = TextureDataFormat::RGBA8);
    };

}

#endif //TEXTURE_H
