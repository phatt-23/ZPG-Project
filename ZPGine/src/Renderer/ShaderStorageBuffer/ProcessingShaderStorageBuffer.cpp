#include "ProcessingShaderStorageBuffer.h"

namespace ZPG
{
    ProcessingShaderStorageBuffer::ProcessingShaderStorageBuffer(int bindingPoint)
        : ShaderStorageBuffer(bindingPoint, sizeof(ProcessingShaderStorageBuffer::InternalLayout))
    {
    }

    void ProcessingShaderStorageBuffer::SetExposure(float exposure) const
    {
        ZPG_SHADER_STORAGE_BUFFER_SET_DATA(&exposure, InternalLayout, Exposure);
    }

    void ProcessingShaderStorageBuffer::SetGamma(float gamma) const
    {
        ZPG_SHADER_STORAGE_BUFFER_SET_DATA(&gamma, InternalLayout, Gamma);
    }

    void ProcessingShaderStorageBuffer::Null()
    {
        byte data[sizeof(InternalLayout)] = {0};
        SetData(data, sizeof(InternalLayout));
    }

}
