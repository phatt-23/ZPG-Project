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
        };

        ProcessingShaderStorageBuffer(int bindingPoint);

        void SetExposure(float exposure) const;
        void SetGamma(float gamma) const;

        const InternalLayout& GetData() const { return m_Data; };

        virtual void Null() override;

    private:
        InternalLayout m_Data;
    };
}
