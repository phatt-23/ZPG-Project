//
// Created by phatt on 10/31/25.
//

#include "OpenGLMapper.h"

#include "Debug/Asserter.h"


namespace ZPG {


OpenGLMapper::OpenGLAttachmentMapping OpenGLMapper::ToGL(const AttachmentType& bufferType) {
    switch (bufferType) {
        case AttachmentType::Color:         return { GL_COLOR_ATTACHMENT0, GL_COLOR_BUFFER_BIT };
        case AttachmentType::DepthStencil:  return { GL_DEPTH_STENCIL_ATTACHMENT, GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT };
        case AttachmentType::Depth:         return { GL_DEPTH_ATTACHMENT, GL_DEPTH_BUFFER_BIT };
        case AttachmentType::Stencil:       return { GL_STENCIL_ATTACHMENT, GL_STENCIL_BUFFER_BIT };
    }

    ZPG_UNREACHABLE();
}

OpenGLMapper::OpenGLFormatMapping OpenGLMapper::ToGL(const DataFormat& format) {
    switch (format) {
        case DataFormat::R8:                 return { GL_R8,                 GL_RED,             GL_UNSIGNED_BYTE,       1 };
        case DataFormat::RG8:                return { GL_RG8,                GL_RG,              GL_UNSIGNED_BYTE,       2 };
        case DataFormat::RGB8:               return { GL_RGB8,               GL_RGB,             GL_UNSIGNED_BYTE,       3 };
        case DataFormat::RGB16F:             return { GL_RGB16F,             GL_RGB,             GL_FLOAT,               6 };
        case DataFormat::RGB32F:             return { GL_RGB32F,             GL_RGB,             GL_FLOAT,               12 };
        case DataFormat::RGBA8:              return { GL_RGBA8,              GL_RGBA,            GL_UNSIGNED_BYTE,       4 };
        case DataFormat::RGBA16F:            return { GL_RGBA16F,            GL_RGBA,            GL_FLOAT,               8 };
        case DataFormat::RGBA32F:            return { GL_RGBA32F,            GL_RGBA,            GL_FLOAT,               16 };
        case DataFormat::Depth24Stencil8:   return { GL_DEPTH24_STENCIL8,   GL_DEPTH_STENCIL,   GL_UNSIGNED_INT_24_8,   4 };
        case DataFormat::Depth32F:           return { GL_DEPTH_COMPONENT32F, GL_DEPTH_COMPONENT, GL_FLOAT,               4 };
        default: return { GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE, 4 };
    }
}

}
