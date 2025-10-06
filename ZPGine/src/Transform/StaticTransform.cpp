#include "StaticTransform.h"
#include "Debug/Asserter.h"

namespace ZPG {

StaticTransform::StaticTransform(bool cached) : m_Cached(cached) {
}

StaticTransform::~StaticTransform() {
}

const glm::mat4& StaticTransform::GetMatrix() {
    ZPG_CORE_ASSERT(m_Cached, "Static transformation's matrix should already be computed.");
    return m_Matrix;
}

}
