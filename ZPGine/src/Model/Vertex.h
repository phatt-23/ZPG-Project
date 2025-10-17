#pragma once

namespace ZPG {

class BufferLayout;

struct Vertex {
    v3 Position;
    v3 Normal;
    v2 TexUV;
    v3 Tangent;

    Vertex(const v3& position, const v3& normal, const v2& texUV, const v3& tangent);
    static BufferLayout GetLayout();
};

}
