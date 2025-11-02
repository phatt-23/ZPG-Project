//
// Created by phatt on 10/24/25.
//

#include "DrawData.h"
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
    , EntitiesStorage   ()
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
    , EntitiesSSBO      (5,
        sizeof(EntitiesStorageBuffer::EntityCount) +
        sizeof(EntitiesStorageBuffer::_pad0) +
        sizeof(EntitiesStorageBuffer::EntityIDs) * s_BatchSize)
{
    // Storages
    {
        LightsStorage.Lights = new LightStruct[LightManager::s_LightCapacity];

        // since there can be only s_BatchSize draw commands
        // in a single batch, we should only need s_BatchSize
        // model matrices
        ModelsStorage.Models = new m4[s_BatchSize];

        EntitiesStorage.EntityIDs = new glm::i32vec4[s_BatchSize];
    }

    const Window& window = Application::Get().GetWindow();

    // G-Buffer initialization
    {
        FrameBufferSpecification GBufferSpec;
        GBufferSpec.Width = window.GetWidth();
        GBufferSpec.Height = window.GetHeight();
        GBufferSpec.Attachments = {
            { TextureDataFormat::Depth24Stencil8, 0 },
            { TextureDataFormat::RGBA32F, 0 }, // pos
            { TextureDataFormat::RGBA32F, 1 }, // normal
            { TextureDataFormat::RGBA32F, 2 }, // albedo metallic
            { TextureDataFormat::RGBA32F, 3 }, // emissive roughness
            { TextureDataFormat::RedInteger, 4 }, // entity id
        };

        GBuffer = FrameBuffer::Create(GBufferSpec);

        // G-Buffer Shader programs and quad
        GPassSP = ShaderProgram::Create("gPass", {
            Shader::Create("assets/shaders/deferred/vert/gPass.vert"),
            Shader::Create("assets/shaders/deferred/frag/gPass.frag")});

        LightingPassShaderProgram = ShaderProgram::Create("LightingPass", {
            Shader::Create("assets/shaders/deferred/vert/LightingPass.vert"),
            Shader::Create("assets/shaders/deferred/frag/LightingPass.frag")});

        QuadVAO = VertexArray::Create({
            VertexBuffer::Create(quadVertices, sizeof(quadVertices), {
                {ShaderDataType::Float2, "a_Pos"},
                {ShaderDataType::Float2, "a_TexCoords"},
            }),
        });

        LightingPassShaderProgram->Bind();
        for (auto& colorAttachment : GBuffer->GetColorTextureAttachments() | std::views::keys)
        {
            std::string uniformName = "g_Color" + std::to_string(colorAttachment.Index);
            LightingPassShaderProgram->SetInt(uniformName, colorAttachment.Index);
        }
        LightingPassShaderProgram->Unbind();
    }


    // Main FBO
    {
        FrameBufferSpecification mainFBOSpec;
        mainFBOSpec.Width = window.GetWidth();
        mainFBOSpec.Height = window.GetHeight();
        mainFBOSpec.Attachments = {
            {TextureDataFormat::RGBA8, 0},      // viewing result
            {TextureDataFormat::RedInteger, 1}, // entity id
            {TextureDataFormat::Depth24Stencil8, 0},
        };

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
    delete[] EntitiesStorage.EntityIDs;
}

}
