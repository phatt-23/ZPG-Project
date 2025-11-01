#pragma once

namespace ZPG {

class PositionComponent {
public:
    explicit PositionComponent(const v3& position = v3(0.f, 0.f, -1.f));
    void Set(const v3& position);
    [[nodiscard]] const v3& Get() const;
private:
    v3 m_Position;
};
    
}
