//
// Created by phatt on 10/29/25.
//

#include "BufferDataFormat.h"

namespace ZPG {

BufferDataFormat::GLFormatMapping BufferDataFormat::ToGL(BufferDataFormat::BufferFormat format) {
    switch (format) {
    case BufferDataFormat::R8:                 return { GL_R8,                 GL_RED,             GL_UNSIGNED_BYTE,       1 };
    case BufferDataFormat::RG8:                return { GL_RG8,                GL_RG,              GL_UNSIGNED_BYTE,       2 };
    case BufferDataFormat::RGB8:               return { GL_RGB8,               GL_RGB,             GL_UNSIGNED_BYTE,       3 };
    case BufferDataFormat::RGB16F:             return { GL_RGB16F,             GL_RGB,             GL_FLOAT,               6 };
    case BufferDataFormat::RGB32F:             return { GL_RGB32F,             GL_RGB,             GL_FLOAT,               12 };
    case BufferDataFormat::RGBA8:              return { GL_RGBA8,              GL_RGBA,            GL_UNSIGNED_BYTE,       4 };
    case BufferDataFormat::RGBA16F:            return { GL_RGBA16F,            GL_RGBA,            GL_FLOAT,               8 };
    case BufferDataFormat::RGBA32F:            return { GL_RGBA32F,            GL_RGBA,            GL_FLOAT,               16 };
    case BufferDataFormat::DEPTH24_STENCIL8:   return { GL_DEPTH24_STENCIL8,   GL_DEPTH_STENCIL,   GL_UNSIGNED_INT_24_8,   4 };
    case BufferDataFormat::DEPTH32F:           return { GL_DEPTH_COMPONENT32F, GL_DEPTH_COMPONENT, GL_FLOAT,               4 };
    default:                                   return { GL_RGBA8,              GL_RGBA,            GL_UNSIGNED_BYTE,       4 };
    }
}

BufferDataFormat::BufferDataFormat(BufferFormat format)
: Format(format)
, Type(DeduceBufferType(format)) {
}

BufferDataFormat::BufferType BufferDataFormat::DeduceBufferType(BufferFormat format) {
    switch (format) {
    case BufferFormat::DEPTH32F: return BufferType::Depth;
    case BufferFormat::DEPTH24_STENCIL8: return BufferType::DepthStencil;
    default: return BufferType::Color;
    }
}
}
