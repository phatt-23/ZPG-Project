#include "DynTranslate.h"
#include <glm/gtc/matrix_transform.hpp>

#include "Core/Timestep.h"

namespace ZPG {

DynTranslate::DynTranslate(
    const v3& startingTranslation, 
    const v3& translationIncrement, 
    const v3& minTranslation, 
    const v3& maxTranslation)
: m_CurrentTranslation(startingTranslation)
, m_TranslationIncrement(translationIncrement)
, m_MinTranslation(minTranslation)
, m_MaxTranslation(maxTranslation) {
    ComputeMatrix();
}

void DynTranslate::Update(Timestep& ts) {
    m_CurrentTranslation += (m_TranslationIncrement * ts.AsSeconds());
    m_CurrentTranslation = glm::clamp(m_CurrentTranslation, m_MinTranslation, m_MaxTranslation);
    ComputeMatrix();
}

void DynTranslate::ComputeMatrix() {
    m_Matrix = glm::translate(m4(1.0f), m_CurrentTranslation);
}

const v3& DynTranslate::GetCurrentTranslation() const { return m_CurrentTranslation; }
const v3& DynTranslate::GetTranslationIncrement() const { return m_TranslationIncrement; }
const v3& DynTranslate::GetMinTranslation() const { return m_MinTranslation; }
const v3& DynTranslate::GetMaxTranslation() const { return m_MaxTranslation; }

void DynTranslate::SetTranslation(const v3& translation) {
    m_CurrentTranslation = translation;
    ComputeMatrix();
}

void DynTranslate::SetTranslationIncrement(const v3& translationIncrement) {
    m_TranslationIncrement = translationIncrement;
}

void DynTranslate::SetMinTranslation(const v3& minTranslation) {
    m_MinTranslation = minTranslation;
}

void DynTranslate::SetMaxTranslation(const v3& maxTranslation) {
    m_MaxTranslation = maxTranslation;
}

}
