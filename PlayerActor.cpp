#include "PlayerActor.h"
//#include "GamePlay.h"
//#include "EnemyActor.h"
#include "CameraComponent.h"
#include "PlayerMove.h"
#include "AnimSpriteComponent.h"
#include "SwordComponent.h"
#include "ArrowComponent.h"

#include "GamePlay.h"

PlayerActor::PlayerActor(Sequence* sequence, Type type)
	: Actor(sequence, type)
	, mSwordComp(nullptr)
{
	Texture2D* tex  = mSequence->getTexture("testPlayerIdle.png");
	mPosition = Vector2{ 100.0f, 200.0f };
	mRectangle = {
		mPosition.x - tex->width / 2.0f,
		mPosition.y - tex->height / 2.0f,
		(float)tex->width,
		(float)tex->height
	};
	
	mAnimsc = new AnimSpriteComponent(this);

	// �A�j���[�V������ǉ�
	std::vector<Texture2D*> idleTexs = { mSequence->getTexture("testPlayerIdle.png") };
	std::vector<Texture2D*> walkTexs = { mSequence->getTexture("testPlayerWalk.png") };
	std::vector<Texture2D*> jumpTexs = { mSequence->getTexture("testPlayerJump.png") };
	std::vector<Texture2D*> dodgeTexs = { mSequence->getTexture("testPlayerDodge.png") };
	std::vector<Texture2D*> chargeTexs = { mSequence->getTexture("testPlayerCharge.png") };
	std::vector<Texture2D*> nAttackTexs = { mSequence->getTexture("testPlayerNormalAttack.png") };
	std::vector<Texture2D*> dAttackTexs = { mSequence->getTexture("testPlayerDodgeAttack.png") };
	std::vector<Texture2D*> cAttackTexs = { mSequence->getTexture("testPlayerChargeAttack.png") };
	// �������́Aplay���ĂԂƂ��Ɏg��
	mAnimsc->addAnimation("Idle", idleTexs);
	mAnimsc->addAnimation("Walk", walkTexs);
	mAnimsc->addAnimation("Jump", jumpTexs);
	mAnimsc->addAnimation("Dodge", dodgeTexs);
	mAnimsc->addAnimation("Charge", chargeTexs);
	mAnimsc->addAnimation("NormalAttack", nAttackTexs);
	mAnimsc->addAnimation("DodgeAttack", dAttackTexs);
	mAnimsc->addAnimation("ChargeAttack", cAttackTexs);
	mAnimsc->play("Idle");

	mCameraComp = new CameraComponent(this);
	mPlayerMove = new PlayerMove(this);
	mSwordComp = new SwordComponent(this);
	mArrowComp = new ArrowComponent(this);
	mWeaponComp = mSwordComp;

	// ��Ԃ��}�b�v�ɓo�^ ��Ԃ͈�C��new,delete����
	// ���R:��Ԑ؂�ւ��̓x��new,delete�̓t���O�����e�[�V�������C�ɂȂ邩��W
	mPlayerStates[PlayerState::Type::Idle] = new Idle(this);
	mPlayerStates[PlayerState::Type::Walk] = new Walk(this);
	mPlayerStates[PlayerState::Type::Jump] = new Jump(this);
	mPlayerStates[PlayerState::Type::Dodge] = new Dodge(this);
	mPlayerStates[PlayerState::Type::Guard] = new Guard(this);
	mPlayerStates[PlayerState::Type::Charge] = new Charge(this);
	mPlayerStates[PlayerState::Type::NormalAttack] = new NormalAttack(this);
	mPlayerStates[PlayerState::Type::DodgeAttack] = new DodgeAttack(this);
	mPlayerStates[PlayerState::Type::ChargeAttack] = new ChargeAttack(this);
	// ���݂̏�Ԃ�ݒ�
	mPlayerState = mPlayerStates[PlayerState::Type::Idle];
}

PlayerActor::~PlayerActor()
{
	for (auto state : mPlayerStates) {
		delete state.second;
	}
}

void PlayerActor::input()
{
	// ����input() : Component��input
	Actor::input();
	mPlayerState->input();
}

void PlayerActor::update()
{
	// ����update() : Component��update
	Actor::update();
	mPlayerState->update();
}

const Camera2D& PlayerActor::getCamera() const
{
	return mCameraComp->getCamera();
}

void PlayerActor::computeRectangle()
{
	mRectangle.x = mPosition.x - mAnimsc->getTexWidth() / 2.0f;
	mRectangle.y = mPosition.y - mAnimsc->getTexHeight() / 2.0f;
}

void PlayerActor::changeState(PlayerState::Type type)
{
	mPlayerState->exit();
	mPlayerState = mPlayerStates[type];
	mPlayerState->enter();
}

//void PlayerActor::onEnterState(PlayerState nextState)
//{
	//// �g������
	//PlayerState lastMoveState = mMoveState;
	//PlayerState lastActionState = mActionState;

	//if (nextState <= ms_jump) {
	//	mMoveState = nextState;
	//	switch (mMoveState)
	//	{
	//	case ms_idle:
	//		mAnimsc->play("Idle"); break;
	//	case ms_jump:
	//		mAnimsc->play("Jump"); break;
	//	case ms_walk:
	//		mAnimsc->play("Walk"); break;
	//	}

	//}
	//else {
	//	mActionState = nextState;
	//	switch (mActionState)
	//	{
	//	case as_idle:
	//		break;
	//	case as_attack:
	//		// charge�U��
	//		if (lastActionState == as_charge) {
	//			mSwordComp->startAttack();
	//		}
	//		// �ʏ�U��
	//		else {
	//			mSwordComp->startAttack();
	//			//mArrowComp->startAttack();
	//		}
	//		break;
	//	case as_guard:
	//		break;
	//	}
	//}
//}

