#pragma once

#include "Models/box.h"
#include "ZPGine.h"

namespace AxisSceneNS {

class AxisLayer : public ZPG::Layer {
public:
    AxisLayer() {}
    ~AxisLayer() override {}
    void OnAttach() override {
        using namespace ZPG;

        m_VAO = VertexArray::Create({
            VertexBuffer::Create(
                BoxModel::boxVertices, 
                sizeof(BoxModel::boxVertices)/sizeof(*BoxModel::boxVertices),
                {
                    {ShaderDataType::Float3, "a_Pos"},
                    {ShaderDataType::Float3, "a_Normal"},
                }
            ),
        });
    }
    void OnUpdate([[maybe_unused]] ZPG::SceneContext& ctx) override {

    }
    void OnRender([[maybe_unused]] const ZPG::RenderContext& ctx) override {
        using namespace ZPG;
        Renderer::BeginDraw(ctx.m_Camera);
        // Renderer::Submit();
        Renderer::EndDraw();
    }
private:
    ZPG::Ref<ZPG::VertexArray> m_VAO;
};

}
