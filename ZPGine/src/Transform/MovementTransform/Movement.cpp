#include "Movement.h"
#include "Core/Timestep.h"
#include "Core/Utility.h"

namespace ZPG
{

    void Movement::Update(Timestep &ts)
    {
        m_CurrentTime = m_CurrentTime + ts.AsSeconds();
    } 

    Movement::Movement(float duration, float currentTime)
        : Transform()
        , m_CurrentTime(currentTime)
        , m_Duration(duration) 
    {
    }

    Movement::~Movement() 
    {
    }

} // namespace ZPG
