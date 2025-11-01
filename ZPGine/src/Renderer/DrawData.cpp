//
// Created by phatt on 10/24/25.
//

#include "DrawData.h"

#include "Light/LightManager.h"
#include "Light/LightStruct.h"
#include "Texture/Texture.h"
#include "Buffer/VertexArray.h"
#include "Core/Application.h"
#include "Shader/Shader.h"
#include "Shader/ShaderDataType.h"
#include "Shader/ShaderProgram.h"
#include "Buffer/BufferLayout.h"
#include "Core/Window.h"
#include "Skybox/Skybox.h"

namespace ZPG {

static constexpr u32 s_BatchSize = 1024;

/**

-----------------------------------------------------------
| ColorAttachment     | 1     | 2     | 3     | 4         |
-----------------------------------------------------------
| Pos                 | pos.x | pos.y | pos.z |           |
| Normal              | nor.x | nor.y | nor.z |           |
| AlbedoMetallic      | alb.r | alb.g | alb.b | metallic  |
| EmissiveRoughness   | emi.r | emi.g | emi.b | roughness |
-----------------------------------------------------------

*/

// quad that fills the entire screen in NDC
constexpr static float quadVertices[] = {
    // positions   // texCoords
    -1.0f,  1.0f,  0.0f, 1.0f,
    -1.0f, -1.0f,  0.0f, 0.0f,
     1.0f, -1.0f,  1.0f, 0.0f,

    -1.0f,  1.0f,  0.0f, 1.0f,
     1.0f, -1.0f,  1.0f, 0.0f,
     1.0f,  1.0f,  1.0f, 1.0f
};


DrawData::DrawData()
    : Batch             (s_BatchSize)
    // Mirrored Shader Storage Buffers on CPU
    , MatricesStorage   ()
    , LightsStorage     ()
    , CameraStorage     ()
    , MaterialStorage   ()
    , ModelsStorage     ()
    // Shader Storage Buffers
    // light has a dynamically sized array, so I can't just use sizeof
    , MatricesSSBO      (0, sizeof(MatricesStorageBuffer))
    , LightsSSBO        (1, 
        sizeof(LightsStorageBuffer::LightCount) + 
        sizeof(LightsStorageBuffer::_pad0) +
        sizeof(LightStruct) * LightManager::s_LightCapacity)
    , CameraSSBO        (2, sizeof(CameraStorageBuffer))
    , MaterialSSBO      (3, sizeof(MaterialStorageBuffer))
    , ModelsSSBO        (4, 
        sizeof(ModelsStorageBuffer::ModelCount) + 
        sizeof(ModelsStorageBuffer::_pad_ModelCount) +
        sizeof(m4) * s_BatchSize)
{
    // Storages
    {
        LightsStorage.Lights = new LightStruct[LightManager::s_LightCapacity];

        // since there can be only s_BatchSize draw commands
        // in a single batch, we should only need s_BatchSize
        // model matrices
        ModelsStorage.Models = new m4[s_BatchSize];
    }

    const Window& window = Application::Get().GetWindow();

    // G-Buffer initialization
    {
        std::unordered_map<std::string, RenderAttachment> attachmentConfigurations;
        attachmentConfigurations["gDepthStencilMap"]       = RenderAttachment( BufferSpecification(DataFormat::Depth24Stencil8), 0 );
        attachmentConfigurations["gPosMap"]                = RenderAttachment( BufferSpecification(DataFormat::RGBA32F), 0 );
        attachmentConfigurations["gNormalMap"]             = RenderAttachment( BufferSpecification(DataFormat::RGBA32F), 1 );
        attachmentConfigurations["gAlbedoMetallicMap"]     = RenderAttachment( BufferSpecification(DataFormat::RGBA32F), 2 );
        attachmentConfigurations["gEmissiveRoughnessMap"]  = RenderAttachment( BufferSpecification(DataFormat::RGBA32F), 3 );

        std::unordered_map<std::string, std::string> attachmentUniformMapping;
        attachmentUniformMapping["gPosMap"]                 = "ug_PosMap";
        attachmentUniformMapping["gNormalMap"]              = "ug_NormalMap";
        attachmentUniformMapping["gAlbedoMetallicMap"]      = "ug_AlbedoMetallicMap";
        attachmentUniformMapping["gEmissiveRoughnessMap"]   = "ug_EmissiveRoughnessMap";

        FrameBufferSpecification GBufferSpec;
        GBufferSpec.Width = window.GetWidth();
        GBufferSpec.Height = window.GetHeight();
        GBufferSpec.Attachments = attachmentConfigurations;

        GBuffer = FrameBuffer::Create(GBufferSpec);

        // G-Buffer Shader programs and quad
        GPassSP = ShaderProgram::Create("gPass", {
                        Shader::Create("assets/shaders/deferred/vert/gPass.vert"),
                        Shader::Create("assets/shaders/deferred/frag/gPass.frag")
                    });

        LightingPassShaderProgram = ShaderProgram::Create("LightingPass", {
                        Shader::Create("assets/shaders/deferred/vert/LightingPass.vert"),
                        Shader::Create("assets/shaders/deferred/frag/LightingPass.frag")
                    });

        QuadVAO = VertexArray::Create({
            VertexBuffer::Create(quadVertices, sizeof(quadVertices), {
                {ShaderDataType::Float2, "a_Pos"},
                {ShaderDataType::Float2, "a_TexCoords"},
            }),
        });

        LightingPassShaderProgram->Bind();
        for (auto& [attachmentName, uniformName] : attachmentUniformMapping) {
            RenderAttachment& attachment = attachmentConfigurations[attachmentName];
            LightingPassShaderProgram->SetInt(uniformName, attachment.BindingPoint);
        }
        LightingPassShaderProgram->Unbind();
    }


    // Main FBO
    {
        FrameBufferSpecification mainFBOSpec;
        mainFBOSpec.Width = window.GetWidth();
        mainFBOSpec.Height = window.GetHeight();
        mainFBOSpec.Attachments["Color"] = RenderAttachment(BufferSpecification(DataFormat::RGBA8), 0);
        mainFBOSpec.Attachments["DepthStencil"] = RenderAttachment(BufferSpecification(DataFormat::Depth24Stencil8), 0);

        MainFBO = FrameBuffer::Create(mainFBOSpec);
    }

    // Default skybox
    {
        CurrentSkybox = Skybox::Create(SkyboxSpecification{ .Directory = "./assets/textures/basic-skybox/" });
    }


}

DrawData::~DrawData() {
    delete[] LightsStorage.Lights;
    delete[] ModelsStorage.Models;
}

}
