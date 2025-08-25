#include "EnemyMove.h"
#include "GamePlay.h"
#include "EnemyActor.h"
#include "PlayerActor.h"

EnemyMove::EnemyMove(EnemyActor* owner)
	: MoveComponent(owner)
	, mEnemy(owner)
	, mAttackRange(5)
{
	mJumpSpeed = -300.0f;
	// �^�[�Q�b�g�w��(EnemyActor������w�肵�Ă�����)
	GamePlay* seq = static_cast<GamePlay*>(mOwner->getSequence());
	mTarget = seq->getPlayer();
}

void EnemyMove::update()
{
	float deltaTime = GetFrameTime();
	
	// �d��(�����ړ�)
	if (mVelocityY < 600.0f) {
		mVelocityY += mGravity;
	}

	Vector2 ownerPos = mOwner->getPosition();
	Vector2 targetPos = mTarget->getPosition();

	// �����˒��O�Ȃ�
	if (abs(ownerPos.x - targetPos.x) > mAttackRange || 
		abs(ownerPos.y - targetPos.y) > mAttackRange) {
		// ���g�̈ʒu�ɑ΂���v���C���[�̈ʒu�̕������擾(���g���Ȃ�-1�A���Ȃ�1�ɐi��)
		int xDirection = ((ownerPos.x - targetPos.x) < 0) - ((ownerPos.x - targetPos.x) > 0);
		mOwner->setForward(xDirection);

		//�����ړ�
		//if()
	}
	else {
		// �Î~���Ă���̂ŁA�U���̎���
		mOwner->setForward(0);
	}

	//�����ړ��̑��x��ݒ�
	int forward = mOwner->getForward();
	mVelocityX = forward * mMoveSpeed;

	//���x���ʒu�ɔ��f
	ownerPos.x += mVelocityX / GetFPS();
	ownerPos.y += mVelocityY / GetFPS();

	mOwner->setPosition(ownerPos);
	// Enemy�̈ړ������S�ɏI��������̈ʒu����ɒ����`�悷�邽�߂�Rectangle�ʒu����
	mOwner->computeRectangle();
}

void EnemyMove::fixFloorCol()
{
	// �߂�����Ƃ肠����
	if(mVelocityY != mJumpSpeed) mVelocityY = 0.0f;
}
