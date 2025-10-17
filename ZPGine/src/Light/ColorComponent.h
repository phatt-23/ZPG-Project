#pragma once

namespace ZPG {

class ColorComponent {
public:
    ColorComponent(const v4& color = v4(1.f, 1.f, 1.f, 1.f));

    void SetColor(const v4& color);
    const v4& GetColor() const;
private:
    v4 m_Color;
};
    
}
