//
// Created by phatt on 9/22/25.
//

#ifndef GRAPHICSCONTEXT_H
#define GRAPHICSCONTEXT_H

namespace ZPG {

class GraphicsContext {
public:
    GraphicsContext() = default;
    virtual ~GraphicsContext() = default;

    virtual void Init() = 0;
    virtual void SwapBuffers() = 0;
private:
};

}

#endif //GRAPHICSCONTEXT_H
