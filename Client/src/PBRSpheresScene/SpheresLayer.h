//
// Created by phatt on 10/16/25.
//

#pragma once

#include "ZPGine.h"

namespace PBRSpheresSceneNS {

class SpheresLayer : public ZPG::Layer {
public:
    void OnAttach() override;
    void OnUpdate(ZPG::SceneContext& ctx) override;
    void OnRender(const ZPG::RenderContext& ctx) override;
private:
};

} // BallsSceneNS


