#pragma once

namespace ZPG {

class Timestep {
public:
    Timestep(f32 delta) : m_Delta(delta) {}

    float AsSeconds() const { return m_Delta; }
    f32 AsMilliSeconds() const { return m_Delta * 1000; }

    operator f32() const { return m_Delta; }
    // operator double() const { return (double)m_Delta; }
private:
    f32 m_Delta;  // in seconds
};

}
