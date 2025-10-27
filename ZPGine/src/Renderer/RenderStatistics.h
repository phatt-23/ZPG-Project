//
// Created by phatt on 10/26/25.
//

#ifndef WORKSPACE_RENDERSTATISTICS_H
#define WORKSPACE_RENDERSTATISTICS_H

namespace ZPG {

struct RenderStatistics {
    u32 FlushCountPerFrame = 0;
    u32 DrawCallCountPerFrame = 0;

    u32 ShaderProgramGroupCount = 0;
    u32 MaterialGroupCount = 0;
    u32 VAOGroupCount = 0;

    void Reset() {
        FlushCountPerFrame = 0;
        DrawCallCountPerFrame = 0;
        ShaderProgramGroupCount = 0;
        MaterialGroupCount = 0;
        VAOGroupCount = 0;
    }
};


}

#endif //WORKSPACE_RENDERSTATISTICS_H