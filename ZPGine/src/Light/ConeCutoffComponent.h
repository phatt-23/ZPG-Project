#pragma once

namespace ZPG {

class ConeCutoffComponent {
public:
    ConeCutoffComponent(f32 innerCutoff, f32 outerCutoff);

    f32 GetInnerCutoff() const;
    void SetInnerCutoff(f32 innerCutoff);
    f32 GetOuterCutoff() const;
    void SetOuterCutoff(f32 outerCutoff);

private:
    f32 m_InnerCutoff;  // in radians
    f32 m_OuterCutoff;
};

}
