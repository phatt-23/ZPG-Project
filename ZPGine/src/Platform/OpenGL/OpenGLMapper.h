//
// Created by phatt on 10/31/25.
//
#pragma once

#include "Buffer/BufferSpecification.h"

namespace ZPG {

    class OpenGLMapper {
    public:

        struct OpenGLAttachmentMapping {
            GLenum Attachment;
            GLenum BufferBit;
        };

        struct OpenGLFormatMapping {
            GLenum InternalFormat;
            GLenum Format;
            GLenum SampleType;
            GLenum SampleTypeSize;
        };

        static OpenGLAttachmentMapping ToGL(const AttachmentType& bufferType);
        static OpenGLFormatMapping ToGL(const DataFormat& format);
    };

}

