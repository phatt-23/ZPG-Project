//
// Created by phatt on 11/3/25.
//

#ifndef WORKSPACE_CUBEMAP_TEXTURE_H
#define WORKSPACE_CUBEMAP_TEXTURE_H
#include "Texture2D.h"

namespace ZPG
{

    enum class CubemapFace
    {
        Right = 0,
        Left = 1,
        Top = 2,
        Bottom = 3,
        Front = 4,
        Back = 5,

        PX = Right,
        NX = Left,
        PY = Top,
        NY = Bottom,
        PZ = Front,
        NZ = Back,
    };

    class TextureCubeMap : public Texture
    {
    public:
        virtual ~TextureCubeMap() override {}

        TextureType GetTextureType() const override { return TextureType::TextureCubeMap; }

        virtual void SetFaceData(CubemapFace face, const void* data, u32 size) = 0;

        virtual u32 GetLength() const = 0;

        virtual void Resize(u32 length) = 0;

        /**
         * Load from 6 separate image files
         */
        static ref<TextureCubeMap> Create(
            const std::string& name,
            const std::array<std::string, 6>& faceFilepaths,
            TextureDataFormat dataFormat = TextureDataFormat::RGBA8);

        /**
         * Create an empty cubemap (for rendering or framebuffer attachment)
         */
        static ref<TextureCubeMap> Create(
            const std::string& name,
            u32 length,
            TextureDataFormat dataFormat = TextureDataFormat::RGBA8);
    };

}

#endif