#include "TransitionRenderPass.h"
#include "Debug/Logger.h"
#include "Renderer/RenderCommand.h"
#include "Renderer/RenderContext.h"

namespace ZPG
{

    void TransitionRenderPass::Init(RenderContext& context) 
    {
    }

    void TransitionRenderPass::Execute(RenderContext& context)
    {
        context.Targets.MainFrameBuffer->Bind();
        RenderCommand::Clear();
        context.Targets.MainFrameBuffer->CopyAttachment(context.Targets.GeometryFrameBuffer, FrameBufferAttachmentType::Depth);
        context.Targets.MainFrameBuffer->Unbind();
    }

}
