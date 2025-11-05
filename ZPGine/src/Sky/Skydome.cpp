#include "Skydome.h"

#include "Debug/Asserter.h"
#include "Platform/OpenGL/OpenGLSkydome.h"
#include "Renderer/Renderer.h"

namespace ZPG
{
    Skydome::~Skydome()
    {

    }

    SkyType Skydome::GetSkyType() const {
        return SkyType::Skydome;
    }

    ref<Skydome> Skydome::Create(const SkydomeSpecification& spec)
    {
        switch (Renderer::GetAPI()) {
            case RendererAPI::None: ZPG_UNREACHABLE("RendererAPI::None is not supported"); return nullptr;
            case RendererAPI::OpenGL: return MakeRef(new OpenGLSkydome(spec));
        }

        ZPG_UNREACHABLE();
    }
}
