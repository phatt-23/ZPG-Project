//
// Created by phatt on 9/22/25.
//

#ifndef GRAPHICS_CONTEXT_H
#define GRAPHICS_CONTEXT_H

namespace ZPG
{

    class GraphicsContext
    {
    public:
        GraphicsContext() = default;
        virtual ~GraphicsContext() = default;

        virtual void Init() = 0;
        virtual void SwapBuffers() = 0;
    };

}

#endif
