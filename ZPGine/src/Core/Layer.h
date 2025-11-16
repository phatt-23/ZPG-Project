//
// Created by phatt on 9/22/25.
//

#ifndef LAYER_H
#define LAYER_H

namespace ZPG
{
    class Timestep;
    class Event;
    class RenderContext;
    class Scene;

    class Layer
    {
    public:
        Layer() {}
        virtual ~Layer() = default;
        virtual void OnAttach() {}
        virtual void OnDetach() {}
        virtual void OnUpdate(Timestep& ts) { ZPG_IM_UNUSED(ts); }
        virtual void OnEvent(Event& event) { ZPG_IM_UNUSED(event); }
        virtual void OnRender() {}
        virtual void OnImGuiRender() {}

        void SetScene(Scene* scene) { m_Scene = scene; }

    protected:
        Scene& GetScene() { return *m_Scene; }

    private:
        Scene* m_Scene = nullptr; // parent scene
    };

}

#endif //LAYER_H
