#include "DynTranslate.h"
#include <glm/gtc/matrix_transform.hpp>

namespace ZPG {

DynTranslate::DynTranslate(
        crVEC3 startingTranslation, 
        crVEC3 translationIncrement, 
        crVEC3 minTranslation, 
        crVEC3 maxTranslation)
: m_CurrentTranslation(startingTranslation)
, m_TranslationIncrement(translationIncrement)
, m_MinTranslation(minTranslation)
, m_MaxTranslation(maxTranslation) {
    ComputeMatrix();
}
void DynTranslate::Update(Timestep ts) {
    m_CurrentTranslation += (m_TranslationIncrement * ts.AsSeconds());
    m_CurrentTranslation = glm::clamp(m_CurrentTranslation, m_MinTranslation, m_MaxTranslation);
    ComputeMatrix();
}
void DynTranslate::ComputeMatrix() {
    m_Matrix = glm::translate(glm::mat4(1.0f), m_CurrentTranslation);
}

}
