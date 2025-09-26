#pragma once

namespace ZPG {
class Timestep {
public:
    Timestep(float delta) : m_Delta(delta) {}
    float AsSeconds() const { return m_Delta; }
    float AsMilliSeconds() const { return m_Delta * 1000; }
    operator float() const { return m_Delta; }
    // operator double() const { return (double)m_Delta; }
private:
    float m_Delta;  // in seconds
};
}
