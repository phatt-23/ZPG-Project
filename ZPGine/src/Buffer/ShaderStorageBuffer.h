//
// Created by phatt on 10/23/25.
//

#ifndef WORKSPACE_SHADER_STORAGE_BUFFER_H
#define WORKSPACE_SHADER_STORAGE_BUFFER_H

namespace ZPG {

class ShaderStorageBuffer {
public:
    ShaderStorageBuffer(u32 bindingPoint, u32 size);
    virtual ~ShaderStorageBuffer();

    void Bind();
    void Unbind();

    void* Map(u32 offset = 0, u32 length = 0);
    void Unmap();

    void SetData(void* data, u32 size, u32 offset = 0);
private:
    u32 m_RendererId;
    u32 m_BindingPoint;
    u32 m_Size;
};

} // ZPG

#endif //WORKSPACE_SHADER_STORAGE_BUFFER_H
