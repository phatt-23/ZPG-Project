//
// Created by phatt on 9/22/25.
//

#ifndef VERTEX_ARRAY_H
#define VERTEX_ARRAY_H

#include "IndexBuffer.h"
#include "VertexBuffer.h"

namespace ZPG {

class VertexArray {
public:
    VertexArray() = default;
    virtual ~VertexArray() = default;

    virtual void Bind() const = 0;
    virtual void Unbind() const = 0;

    /**
     * @param vertexBuffer VBO with its own local buffer layout, the layout is "added" to other VBO layouts in this VAO.
     * Each VBO must have the same number of vertices inside it.
     * If VAO consists of more VBOs, then they get indexed like this: (VBO1[i], VBO2[i], ..., VBOn[i]).
     * Therefore, they must all have the same vertex count.
     */
    virtual void AddVertexBuffer(const ref<VertexBuffer>& vertexBuffer) = 0;
    virtual void SetIndexBuffer(const ref<IndexBuffer>& indexBuffer) = 0;

    virtual const std::vector<ref<VertexBuffer>>& GetVertexBuffers() const = 0;
    virtual const ref<IndexBuffer>& GetIndexBuffer() const = 0;

    virtual bool HasIndexBuffer() const = 0;

    virtual u32 GetVertexCount() const = 0;

    static ref<VertexArray> Create();
    static ref<VertexArray> Create(const std::vector<ref<VertexBuffer>>& vertexBuffers);
    static ref<VertexArray> Create(
        const std::vector<ref<VertexBuffer>>& vertexBuffers,
        const ref<IndexBuffer>& indexBuffer);
private:
};

}

#endif
