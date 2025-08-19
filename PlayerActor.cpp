#include "PlayerActor.h"
//#include "GamePlay.h"
//#include "EnemyActor.h"
#include "CameraComponent.h"
#include "PlayerMove.h"

PlayerActor::PlayerActor(Sequence* sequence)
	: Actor(sequence)
{
	mTexture = LoadTexture("testPlayer.png");
	mPosition = Vector2{ 0.0f, 0.0f };
	mRectangle = {
		mPosition.x - mTexture.width / 2.0f,
		mPosition.y - mTexture.height / 2.0f,
		(float)mTexture.width,
		(float)mTexture.height
	};

	mCameraComp = new CameraComponent(this);
	mPlayerMove = new PlayerMove(this);
}

void PlayerActor::input()
{
	// ����input() : Component��input
	Actor::input();
	
	//// AD : ���Ɉړ�
	//if (IsKeyDown(KEY_A)) {
	//	//mPosition.x -= 180 * GetFrameTime();
	//	mPosition.x -= 3;
	//}
	//if (IsKeyDown(KEY_D)) {
	//	//mPosition.x += 180 * GetFrameTime();
	//	mPosition.x += 3;
	//}
	//// Space : �W�����v
	//if (IsKeyDown(KEY_SPACE) && !mJumping  )
	//{
	//	// �W�����v
	//}



}

void PlayerActor::update()
{
	// ����update() : Component��update
	Actor::update();
	// �Čv�Z
	computeRectangle();
}

void PlayerActor::draw()
{
	Rectangle src = { 0, 0, (float)mTexture.width, (float)mTexture.height };
	Rectangle dst = { mPosition.x, mPosition.y, (float)mTexture.width, (float)mTexture.height };
	Vector2 origin = { mTexture.width / 2.0f, mTexture.height / 2.0f };
	DrawTexturePro(mTexture, src, dst, origin, 0.0f, WHITE);
}

const Camera2D& PlayerActor::getCamera() const
{
	return mCameraComp->getCamera();
}

PlayerMove& PlayerActor::getPlayerMove()
{
	return *mPlayerMove;
}

void PlayerActor::computeRectangle()
{
	mRectangle.x = mPosition.x - mTexture.width / 2.0f;
	mRectangle.y = mPosition.y - mTexture.height / 2.0f;
}
