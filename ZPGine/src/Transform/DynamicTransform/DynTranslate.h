#pragma once

#include "Transform/DynTransform.h"

namespace ZPG {

class Timestep;

class DynTranslate : public DynamicTransform {
public:
    DynTranslate(
        const v3& startingTranslation, 
        const v3& translationIncrement, 
        const v3& minTranslation, 
        const v3& maxTranslation);

    void Update(Timestep& ts) override;
    void ComputeMatrix() override;

    const v3& GetCurrentTranslation() const;
    const v3& GetTranslationIncrement() const;
    const v3& GetMinTranslation() const;
    const v3& GetMaxTranslation() const;

    void SetTranslation(const v3& translation);
    void SetTranslationIncrement(const v3& translationIncrement);
    void SetMinTranslation(const v3& minTranslation);
    void SetMaxTranslation(const v3& maxTranslation);
private:
    v3 m_CurrentTranslation;
    v3 m_TranslationIncrement;
    v3 m_MinTranslation;
    v3 m_MaxTranslation;
};

}
