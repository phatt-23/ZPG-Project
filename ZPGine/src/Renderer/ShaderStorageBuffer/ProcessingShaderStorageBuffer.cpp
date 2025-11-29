#include "ProcessingShaderStorageBuffer.h"

namespace ZPG
{
    ProcessingShaderStorageBuffer::ProcessingShaderStorageBuffer(int bindingPoint)
        : ShaderStorageBuffer(bindingPoint, sizeof(ProcessingShaderStorageBuffer::InternalLayout))
    {
    }

    void ProcessingShaderStorageBuffer::SetExposure(float exposure)
    {
        m_Data.Exposure = std::max(exposure, 0.0f);
        ZPG_SHADER_STORAGE_BUFFER_SET_DATA(&m_Data.Exposure, InternalLayout, Exposure);
    }

    void ProcessingShaderStorageBuffer::SetGamma(float gamma)
    {
        m_Data.Gamma = std::max(gamma, 0.0f);
        ZPG_SHADER_STORAGE_BUFFER_SET_DATA(&m_Data.Gamma, InternalLayout, Gamma);
    }

    void ProcessingShaderStorageBuffer::Null()
    {
        byte data[sizeof(InternalLayout)] = {0};
        SetData(data, sizeof(InternalLayout));
    }

    void ProcessingShaderStorageBuffer::SetBloomAmount(int bloomAmount)
    {
        m_Data.BloomAmount = std::max(bloomAmount, 0);
    }

    int ProcessingShaderStorageBuffer::GetBloomAmount() const
    {
        return m_Data.BloomAmount;
    }

    float ProcessingShaderStorageBuffer::GetExposure() const
    {
        return m_Data.Exposure;
    }

    float ProcessingShaderStorageBuffer::GetGamma() const
    {
        return m_Data.Gamma;
    }

}
