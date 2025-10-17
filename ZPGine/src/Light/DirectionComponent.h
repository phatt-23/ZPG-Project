#pragma once

namespace ZPG {

class DirectionComponent {
public:
    DirectionComponent(const v3& direction = v3(0.f, 0.f, -1.f));
    void SetDirection(const v3& direction);
    const v3& GetDirection() const;
private:
    v3 m_Direction;
};
    
}
