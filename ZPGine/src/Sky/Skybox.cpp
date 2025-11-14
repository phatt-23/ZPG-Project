//
// Created by phatt on 10/17/25.
//

#include "Skybox.h"

#include "Buffer/BufferLayout.h"
#include "Buffer/VertexArray.h"
#include "Debug/Asserter.h"
#include "Platform/OpenGL/OpenGLSkybox.h"
#include "Renderer/RendererAPI.h"
#include "Shader/Shader.h"
#include "Shader/ShaderProgram.h"
#include "stb_image/stb_image.h"

namespace ZPG {

Skybox::~Skybox() {
}

SkyType Skybox::GetSkyType() const {
    return SkyType::Skybox;
}

ref<Skybox> Skybox::Create(const SkyboxSpecification& spec)
{
    switch (RendererAPI::GetAPI())
    {
        case RendererAPI::None: ZPG_UNREACHABLE("RendererAPI::None is not supported"); break;
        case RendererAPI::OpenGL: return MakeRef(new OpenGLSkybox(spec));
    }

    ZPG_UNREACHABLE();
    return nullptr;
}

} // ZPG