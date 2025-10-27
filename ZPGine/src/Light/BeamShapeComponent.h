#pragma once

namespace ZPG {

class BeamShapeComponent {
public:
    BeamShapeComponent(f32 size, f32 blend);
    
    f32 GetSize() const;
    f32 GetBlend() const;

    void SetSize(f32 size);
    void SetBlend(f32 blend);

    f32 GetSizeCos() const { return m_SizeCos; };
    f32 GetBlendCos() const { return m_BlendCos;}

private:
    f32 m_Size;  // in angles, the angle between the direction vector and the side of the cone
    f32 m_Blend; // 0 to 1, 0 means sharp edges, 1 means blurred to the core of the cone

    f32 m_SizeCos;
    f32 m_BlendCos;
};

}
