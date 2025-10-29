//
// Created by phatt on 10/24/25.
//

#ifndef WORKSPACE_DRAWDATA_H
#define WORKSPACE_DRAWDATA_H
#include "Buffer/FrameBuffer.h"
#include "RenderBatch.h"
#include "Buffer/ShaderStorageBuffer.h"

namespace ZPG {
struct BufferDataFormat;
struct LightStruct;

struct DrawData {
    DrawData();
    ~DrawData();

    /**
     * Memory layouts of shader storage buffers.
     */

    struct alignas(sizeof(v4)) MatricesStorageBuffer {
        m4 View;
        m4 Proj;
        m4 ViewProj;
    };

    struct alignas(sizeof(v4)) LightsStorageBuffer {
        i32 LightCount;
        f32 _pad0[3];

        LightStruct* Lights;  // dynamically sized array, requires special treatment + must come last
    };

    struct alignas(sizeof(v4)) CameraStorageBuffer {
        v3 CameraPos;
        f32 _pad0[1];
    };

    struct alignas(sizeof(v4)) MaterialStorageBuffer {
        v4 Albedo;
        v4 Emissive;
        f32 Roughness;
        f32 Metallic;
        f32 _pad0[2];
    };

    struct alignas(sizeof(v4)) ModelsStorageBuffer {
        int ModelCount;
        f32 _pad_ModelCount[3];

        m4* Models;
    };

    /**
     *  Batch of some fixed size.
     */

    RenderBatch Batch;

    /**
     * Mirror of Shader Storage Buffers data that is here on the CPU.
     */

    MatricesStorageBuffer MatricesStorage;
    LightsStorageBuffer LightsStorage;
    CameraStorageBuffer CameraStorage;
    MaterialStorageBuffer MaterialStorage;
    ModelsStorageBuffer ModelsStorage;

    /**
     * Actual Shader Storage Buffers (on GPU).
     */

    ShaderStorageBuffer MatricesSSBO;
    ShaderStorageBuffer LightsSSBO;
    ShaderStorageBuffer CameraSSBO;
    ShaderStorageBuffer MaterialSSBO;
    ShaderStorageBuffer ModelsSSBO;

    /**
     * Framebuffer used for rendering only the geometry of a scene.
     */

    ref<FrameBuffer> GBuffer;
    ref<ShaderProgram> GPassSP;
    ref<ShaderProgram> LightingPassSP;

    /**
     * QuadVAO
     */
    ref<VertexArray> QuadVAO;

private:
    static std::vector<std::tuple<Attachment, std::string, BufferDataFormat>> s_TexAttachments;
    static std::vector<std::tuple<std::string, std::string, u32>> s_TexAttachmentsBindings;
    static std::vector<std::tuple<Attachment, std::string, BufferDataFormat>> s_RenderBufferAttachments;
};

}


#endif //WORKSPACE_DRAWDATA_H
