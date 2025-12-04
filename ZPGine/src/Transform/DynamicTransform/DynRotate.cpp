#include "DynRotate.h"
#include "Core/Timestep.h"
#include "Profiling/Instrumentor.h"

namespace ZPG
{

    DynRotate::DynRotate(
        f32 startingRotationDeg,
        f32 rotationSpeedDeg,
        const v3& rotationAxis)
        : m_CurrentRotationDeg(startingRotationDeg)
        , m_RotationSpeedDeg(rotationSpeedDeg)
        , m_RotationAxis(glm::normalize(rotationAxis))
    {
        ZPG_PROFILE_FUNCTION();
        ComputeMatrix();
    }

    void DynRotate::Update(Timestep& ts)
    {
        ZPG_PROFILE_FUNCTION();
        m_CurrentRotationDeg += m_RotationSpeedDeg * ts.AsSeconds();

        if (m_CurrentRotationDeg < 0.0f) {
            m_CurrentRotationDeg += 360.0f;
        }
        else if (m_CurrentRotationDeg >= 360.0f) {
            m_CurrentRotationDeg -= 360.0f;
        }

        ComputeMatrix();
    }

    void DynRotate::ComputeMatrix()
    {
        ZPG_PROFILE_FUNCTION();
        m4 R = glm::rotate(m4(1.0f), glm::radians(m_CurrentRotationDeg), m_RotationAxis);
        m_Matrix = R;
    }

    f32 DynRotate::GetCurrentRotationDeg() const
    {
        ZPG_PROFILE_FUNCTION();
        return m_CurrentRotationDeg;
    }

    f32 DynRotate::GetRotationSpeedDeg() const
    {
        ZPG_PROFILE_FUNCTION();
        return m_RotationSpeedDeg;
    }

    const v3& DynRotate::GetRotationAxis() const
    {
        ZPG_PROFILE_FUNCTION();
        return m_RotationAxis;
    }

    void DynRotate::SetRotationSpeedDeg(f32 rotationSpeedDeg)
    {
        ZPG_PROFILE_FUNCTION();
        m_RotationSpeedDeg = rotationSpeedDeg;
    }

    void DynRotate::SetRotationAxis(const v3& rotationAxis)
    {
        ZPG_PROFILE_FUNCTION();
        m_RotationAxis = glm::normalize(rotationAxis);
    }

    void DynRotate::SetRotationAxis(f32 x, f32 y, f32 z)
    {
        ZPG_PROFILE_FUNCTION();
        m_RotationAxis = glm::normalize(v3(x, y, z));
    }

}
