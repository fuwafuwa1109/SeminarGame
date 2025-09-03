#pragma once
#include "MoveComponent.h"

/// <summary>
/// Player�̈ړ����i��
/// Player��State��ύX�����������
/// </summary>
class PlayerControl :
    public MoveComponent
{
public:
    PlayerControl(class PlayerActor* owner);
    void input()override;
    void update()override;
    void fixFloorCol();

    float getAttackTime() const { return mAttackTime; }

private:
    void StateDraw(); // �f�o�b�O�p�̊֐�
    class PlayerActor* mPlayer; // mOwner�Ɠ���(�^���Ⴄ)
    // �U���p ����͖���������
    float mAttackTimer;
    float mAttackTime;  // �ʏ�U���̎���
    // ���ߍU���p
    float mChargeTimer;
    float mChargeTime;
    // ���p
    float mLastPressTimeD;
    float mLastPressTimeA;
    float mDodgeTime;
    float mDodgeTimer;
    float mDodgeSpeed;
};
