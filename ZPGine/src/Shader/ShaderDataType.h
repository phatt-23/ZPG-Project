#pragma once

namespace ZPG {

struct ShaderDataType {
    enum Type {
        None = 0,
        Bool,
        Float, Float2, Float3, Float4,
        Int, Int2, Int3, Int4,
        Mat3, Mat4,
        Texture2D, 
    };

    static unsigned int Sizeof(Type type);
    static GLenum ToOpenGLType(Type type);
};

}
