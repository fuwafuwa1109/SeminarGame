#include "PlayerMove.h"
#include <raylib.h>
#include "Actor.h"

PlayerMove::PlayerMove(Actor* actor)
	: MoveComponent(actor)
{
}

void PlayerMove::input()
{
	Vector2 pos = mOwner->getPosition();
	
	// �����ݒ�
	mVelocityX = 0.0f;
	// �d�͂����̂܂ܓ������Ƃ�
	if (mVelocityY > 0.0f && !mIsJumping) {
		mIsJumping = true;
	}

	// AD : ���Ɉړ�
	if (IsKeyDown(KEY_A)) {
		mVelocityX -= mMoveSpeed * GetFrameTime();
	}
	if (IsKeyDown(KEY_D)) {
		mVelocityX += mMoveSpeed * GetFrameTime();
	}
	// Space : �W�����v
	if (IsKeyPressed(KEY_SPACE) && !mIsJumping)
	{
		mIsJumping = true;
		mVelocityY = mJumpSpeed;
	}
}

void PlayerMove::update()
{
	// �d��
	MoveComponent::update();
	// ���x��ݒ�
	Vector2 pos = mOwner->getPosition();
	pos.x += mVelocityX;
	pos.y += mVelocityY;
	// �������ɂԂ���Ƃ�,velocityY��GamePlay�N���X��
	// updateCollision�֐���0�ɖ߂����	

	mOwner->setPosition(pos);
}
