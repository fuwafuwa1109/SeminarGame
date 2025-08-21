#pragma once
#include "Component.h"

/// <summary>
/// PlayerControl,EnemyMove�̊��
/// </summary>
class MoveComponent :
    public Component
{
public:
    MoveComponent(class Actor* owner);
    void update() override {} ;

    // �ȉ��ꉞ�p��
    float getMoveSpeed() const { return mMoveSpeed; }
    float getJumpSpeed() const { return mJumpSpeed; }
    float getVelocityY() const { return mVelocityY; }
    bool isJumping() const { return mIsJumping; }
    void setMoveSpped(float speed) { mMoveSpeed = speed; }
    void setJumpSpped(float speed) { mJumpSpeed = speed; }
    void setVelocityY(float velocity) { mVelocityY = velocity; }
    void setJumping(bool jumping) { mIsJumping = jumping; }

protected:
    float mGravity;     // �d��
    float mMoveSpeed;   // �������̈ړ����x
    float mVelocityX;   // �������̑��x
    float mJumpSpeed;   // �W�����v�̏����x
    float mVelocityY;   // �c�����̑��x
    bool mIsJumping;
};

