//
// Created by phatt on 10/24/25.
//

#include "DrawData.h"

#include "Light/LightManager.h"
#include "Light/LightStruct.h"
#include "Texture/Texture.h"
#include "Buffer/RenderBuffer.h"
#include "Buffer/VertexArray.h"
#include "Core/Application.h"
#include "Core/Window.h"
#include "Shader/Shader.h"
#include "Shader/ShaderDataType.h"
#include "Shader/ShaderProgram.h"
#include "Buffer/BufferLayout.h"

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

std::vector<std::tuple<Attachment, std::string, BufferDataFormat>> DrawData::s_TexAttachments = {
    {   {Attachment::DepthStencil,    0},   "gDepthStencil",    BufferDataFormat::DEPTH24_STENCIL8},

    {   {Attachment::Color,           0},   "gPosMap",          BufferDataFormat::RGBA32F },
    {   {Attachment::Color,           1},   "gNormalMap",       BufferDataFormat::RGBA32F },
    {   {Attachment::Color,           2},   "gAlbedoMap",       BufferDataFormat::RGBA32F },
    {   {Attachment::Color,           3},   "gEmissiveMap",     BufferDataFormat::RGBA32F },
};

std::vector<std::tuple<std::string, std::string, u32>> DrawData::s_TexAttachmentsBindings = {
    {   "gPosMap",                  "u_gPosMap",        0   },
    {   "gNormalMap",               "u_gNormalMap",     1   },
    {   "gAlbedoMetallicMap",       "u_gAlbedoMap",     2   },
    {   "gEmissiveRoughnessMap",    "u_gEmissiveMap",   3   },
};

std::vector<std::tuple<Attachment, std::string, BufferDataFormat>> DrawData::s_RenderBufferAttachments = {
    // {   {Attachment::DepthStencil,    0},   "gDepthStencil",            BufferDataFormat::DEPTH24_STENCIL8  },
};


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
        sizeof(m4) * s_BatchSize) {
    LightsStorage.Lights = new LightStruct[LightManager::s_LightCapacity];

    // since there can be only s_BatchSize draw commands
    // in a single batch, we should only need s_BatchSize
    // model matrices
    ModelsStorage.Models = new m4[s_BatchSize];


    // G-Buffer initialization

    const Window& window = Application::Get().GetWindow();

    std::unordered_map<Attachment, ref<Texture>> gBufferTexAttachments;
    std::unordered_map<Attachment, ref<RenderBuffer>> gBufferRBOAttachments;

    for (auto& [attachment, name, format] : s_TexAttachments) {
        gBufferTexAttachments[attachment] = Texture::Create(name, window.GetWidth(), window.GetHeight(), format);
    }

    for (auto& [attachment, name, format] : s_RenderBufferAttachments) {
        gBufferRBOAttachments[attachment] = RenderBuffer::Create(window.GetWidth(), window.GetHeight(), format);
    }

    GBuffer = MakeRef(new FrameBuffer(gBufferTexAttachments, gBufferRBOAttachments));

    GPassSP = ShaderProgram::Create("gPass",
                {
                    Shader::Create("assets/shaders/deferred/vert/gPass.vert"),
                    Shader::Create("assets/shaders/deferred/frag/gPass.frag")
                });

    LightingPassSP = ShaderProgram::Create("LightingPass",
                {
                    Shader::Create("assets/shaders/deferred/vert/LightingPass.vert"),
                    Shader::Create("assets/shaders/deferred/frag/LightingPass.frag")
                });

    QuadVAO = VertexArray::Create({
        VertexBuffer::Create(quadVertices, sizeof(quadVertices), {
            {ShaderDataType::Float2, "a_Pos"},
            {ShaderDataType::Float2, "a_TexCoords"},
        }),
    });

    LightingPassSP->Bind();
    for (auto& [texName, uniName, slot] : s_TexAttachmentsBindings) {
        LightingPassSP->SetInt(uniName, slot);
    }
}

DrawData::~DrawData() {
    delete[] LightsStorage.Lights;
    delete[] ModelsStorage.Models;
}

}
