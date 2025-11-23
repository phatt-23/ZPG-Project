#include "Transform/MovementTransform/Movement.h"

namespace ZPG
{
    class PolyLineMovement : public Movement
    {
    public:
        PolyLineMovement(const vec<v3>& points, 
                         float duration, 
                         MovementMode mode = MovementMode::Once);
        virtual ~PolyLineMovement() override = default;

        void Update(Timestep &ts) override;
        
        v3 GetCurrentPosition() override;
        const vec<v3>& GetPoints() const { return m_Points; }
        void SetPoints(const vec<v3>& points) { m_Points = points; }

    private:
        vec<v3> m_Points;
        std::vector<float> m_Lengths;
        float m_TotalLength = 0.0;
    };
}
