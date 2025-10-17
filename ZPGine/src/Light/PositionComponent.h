#pragma once

namespace ZPG {

class PositionComponent {
public:
    PositionComponent(const v3& position = v3(0.f, 0.f, -1.f));
    void SetPosition(const v3& position);
    const v3& GetPosition() const;
private:
    v3 m_Position;
};
    
}
