#pragma once
#include "Actor.h"

/// <summary>
/// �G�A�N�^�[
/// �K�v�ɉ����Ă܂��p������
/// </summary>
class EnemyActor :
    public Actor
{
public:
    EnemyActor(class Sequence* sequence, Type type);
    ~EnemyActor();

    void update() override;
private:
    class SpriteComponent* mSpriteComp;
};

