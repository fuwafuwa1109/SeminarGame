#include "PlayerActor.h"
//#include "GamePlay.h"
//#include "EnemyActor.h"
#include "CameraComponent.h"
#include "PlayerControl.h"
#include "AnimSpriteComponent.h"
#include "SwordComponent.h"

#include "GamePlay.h"

PlayerActor::PlayerActor(Sequence* sequence, Type type)
	: Actor(sequence, type)
	, mMoveState(ms_idle)
	, mActionState(as_idle)
	, mSwordComp(nullptr)
	, mAttackTime(0.0f)
	, mAttackTimer(0.0f)
{
	Texture2D tex  = mSequence->getTexture("testPlayerIdle.png");
	mPosition = Vector2{ 100.0f, 200.0f };
	mRectangle = {
		mPosition.x - tex.width / 2.0f,
		mPosition.y - tex.height / 2.0f,
		(float)tex.width,
		(float)tex.height
	};

	mAnimsc = new AnimSpriteComponent(this);
	std::vector<Texture2D> texs = {
		tex,
		mSequence->getTexture("testPlayerJump.png"),
		mSequence->getTexture("testPlayerWalk.png"),
		mSequence->getTexture("testPlayerDash.png"),
	};
	// SetTeture��AnimSpriteComponent�̊֐��ň��s��
	mAnimsc->setAnimTextures(texs);
	mAnimsc->play(0, 0, true);	// Idle��Ԃ̊G

	mCameraComp = new CameraComponent(this);
	mPlayerControl = new PlayerControl(this);
	mSwordComp = new SwordComponent(this);
}

void PlayerActor::input()
{
	// ����input() : Component��input
	Actor::input();
}

void PlayerActor::update()
{
	// ����update() : Component��update
	Actor::update();
}

const Camera2D& PlayerActor::getCamera() const
{
	return mCameraComp->getCamera();
}

PlayerControl& PlayerActor::getPlayerControl()
{
	return *mPlayerControl;
}

void PlayerActor::computeRectangle()
{
	mRectangle.x = mPosition.x - mAnimsc->getTexWidth() / 2.0f;
	mRectangle.y = mPosition.y - mAnimsc->getTexHeight() / 2.0f;
}

void PlayerActor::changeState(PlayerState state)
{
	onExitState(state);
	onEnterState(state);
}

void PlayerActor::onExitState(PlayerState nextState)
{
	if (nextState <= ms_jump) {
		switch (mMoveState)
		{
		case ms_idle:
			break;
		case ms_walk:
			break;
		case ms_dash:
			break;
		case ms_jump:
			break;
		}
	}
	else {
		switch (mActionState)
		{
		case as_idle:
			break;
		case as_attack:
 			mSwordComp->endAttack();
			break;
		case as_guard:
			break;
		}
	}
}

void PlayerActor::onEnterState(PlayerState nextState)
{
	if (nextState <= ms_jump) {
		mMoveState = nextState;
		switch (mMoveState)
		{
		case ms_idle:
			mAnimsc->play(0, 0, true); break;
		case ms_jump:
			mAnimsc->play(1, 1, true); break;
		case ms_walk:
			mAnimsc->play(2, 2, true); break;
		case ms_dash:
			mAnimsc->play(3, 3, true); break;
		}

	}
	else {
		mActionState = nextState;
		switch (mActionState)
		{
		case as_idle:
			break;
		case as_attack:
			// �����new����
			mSwordComp->startAttack(0, 9, mPlayerControl->getAttackTime());
			break;
		case as_guard:
			break;
		}
	}
}

