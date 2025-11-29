#pragma once

#include "Buffer/ShaderStorageBuffer.h"

namespace ZPG
{
    class ProcessingShaderStorageBuffer : public ShaderStorageBuffer
    {
    public:
        struct InternalLayout
        {
            float Exposure = 1.0f;
            float Gamma = 2.2f;
            int BloomAmount = 2;
        };

        ProcessingShaderStorageBuffer(int bindingPoint);

        float GetExposure() const;
        float GetGamma() const;
        int GetBloomAmount() const;
        void SetExposure(float exposure);
        void SetGamma(float gamma);
        void SetBloomAmount(int bloomAmount);

        const InternalLayout& GetData() const { return m_Data; };

        virtual void Null() override;

    private:
        InternalLayout m_Data;
    };
}
