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
    PlayerControl(class PlayerActor* actor);
    void input()override;
    void update()override;
    void fixFloorCol();

private:
    void StateDraw(); // �f�o�b�O�p�̊֐�
    class PlayerActor* mPlayer; // mOwner�Ɠ���(�^���Ⴄ)
    bool mIsGuarding;
    float mDashSpeed;
    float actionTimer;
};
