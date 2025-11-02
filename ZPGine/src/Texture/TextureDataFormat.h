//
// Created by phatt on 11/2/25.
//

#ifndef WORKSPACE_TEXTURE_DATA_FORMAT_H
#define WORKSPACE_TEXTURE_DATA_FORMAT_H

enum class TextureDataFormat {
    None = 0,

    R8,

    RG8,

    RGB8,
    RGB16F,
    RGB32F,

    RGBA8,
    RGBA16F,
    RGBA32F,

    RedInteger,

    Depth24Stencil8,
    Depth32F,
};

#endif