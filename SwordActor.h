#pragma once
#include "WeaponActor.h"

/// <summary>
/// �ߐڕ���F��
/// </summary>
class SwordActor :
    public WeaponActor
{
public:
    SwordActor(class Sequence* sequence, Type type, Type owner);

    void input() override;
    void update() override;

    void onStartAttack() override;
    void computeRectangle() override;

private:
    class AnimSpriteComponent* mAnimsc;
    float mColWidth;
    float mColHeight;
};

