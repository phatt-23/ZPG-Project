//
// Created by phatt on 10/18/25.
//

#ifndef UNIFORM_BUFFER_H
#define UNIFORM_BUFFER_H


namespace ZPG {

/*
 *
 * Uniform buffer object for storing global data.
 */
class UniformBuffer {
public:
    UniformBuffer(u32 bindingPoint, u32 size);
    virtual ~UniformBuffer();

    void Bind();
    void Unbind();

    void SetData(void* data, u32 size, u32 offset = 0);

private:
    u32 m_RendererId;
    u32 m_BindingPoint;
    u32 m_Size;
};

}

#endif
