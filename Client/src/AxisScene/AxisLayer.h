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

        auto VAO = VertexArray::Create({
            VertexBuffer::Create(
                BoxModel::boxVertices, 
                sizeof(BoxModel::boxVertices),
                {
                    {ShaderDataType::Float3, "a_Pos"},
                    {ShaderDataType::Float3, "a_Normal"},
                    {ShaderDataType::Float2, "a_UV"},
                }
            ),
        }, IndexBuffer::Create(BoxModel::boxIndices, ZPG_ARRAY_LENGTH(BoxModel::boxIndices)));

        m_Model = CreateScope<Model>(new Model({
            CreateRef<Mesh>(VAO, ScaleTransform(glm::vec3(10.0, 0.1, 0.1)).GetMatrix()),
            CreateRef<Mesh>(VAO, ScaleTransform(glm::vec3(0.1, 10.0, 0.1)).GetMatrix()),
            CreateRef<Mesh>(VAO, ScaleTransform(glm::vec3(0.1, 0.1, 10.0)).GetMatrix()),
        }));
    }
    void OnUpdate([[maybe_unused]] ZPG::SceneContext& ctx) override {

    }
    void OnRender([[maybe_unused]] const ZPG::RenderContext& ctx) override {
        using namespace ZPG;
        Renderer::BeginDraw(ctx.m_Camera);
            Renderer::Submit(*m_ResourceManager.GetShaderProgram("basic_single_color"), *m_Model);
        Renderer::EndDraw();
    }
private:
    ZPG::Scope<ZPG::Model> m_Model;
};

}
