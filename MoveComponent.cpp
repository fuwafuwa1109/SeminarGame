#include "MoveComponent.h"
#include "Actor.h"

MoveComponent::MoveComponent(Actor* owner)
	: Component(owner)
	, mGravity(1.0f)
	, mMoveSpeed(180.0f)
	, mJumpSpeed(-10.0f)
	, mVelocityY(0.0f)
	, mIsJumping(false)
{
}

void MoveComponent::update()
{
	// �d�͂͗^����(�^�������Ȃ��ꍇ�͂���update���Ă΂Ȃ�)
	if (mVelocityY < 10) {
		mVelocityY += mGravity;
	}
}
