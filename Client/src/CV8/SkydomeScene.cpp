#include "SkydomeScene.h"

using namespace ZPG;

namespace CV8
{

    SkydomeScene::SkydomeScene()
        : m_LocalResources()
    {
    }

    void SkydomeScene::OnAttach() {
        m_LocalResources.LoadModel("Skydome", "./assets/models/skydome/skydome.obj");


        SkydomeSpecification skySpec;
        skySpec.TexturePath = "./assets/textures/skydome/fulldome3.jpg";

        ref<Skydome> skydome = Skydome::Create(skySpec);
        Renderer::SetSkydome(skydome);

        GetLightManager().AddLight(new AmbientLight(ColorComponent(v4(1.0))));

        ref<TransformGroup> transform = TransformGroup::Build()
            .Compose();

        GetEntityManager().AddEntity(new Entity(m_LocalResources.GetModel("Skydome"), transform));
    }

}
