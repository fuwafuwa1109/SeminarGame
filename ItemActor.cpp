#include "ItemActor.h"
#include "GamePlay.h"
#include "SoundSystem.h"

#include "SpriteComponent.h"
// ÉvÉåÉCÉÑÅ[
#include "PlayerActor.h"
#include "HpComponent.h"
#include "PlayerMove.h"

ItemActor::ItemActor(Sequence* sequence)
	: Actor(sequence, Type::Eitem)
{
	mSpriteComp = new SpriteComponent(this);
}

void ItemActor::update()
{
	Actor::update();
	
	Rectangle playerRec = static_cast<GamePlay*>(mSequence)->getPlayer()->getRectangle();
	if (CheckCollisionRecs(playerRec, mRectangle)) {
		onAcquired();
		setState(Edead);
	}
}

void ItemActor::computeRectangle()
{
	mRectangle.x = mPosition.x - mRectangle.width / 2.0f;
	mRectangle.y = mPosition.y - mRectangle.height / 2.0f;
}

HealingItem::HealingItem(Sequence* sequence)
	: ItemActor(sequence)
	, mHealAmount(20.0f)
{
	Texture2D* tex = mSequence->getTexture("Assets/HealingItem.png");
	mSpriteComp->setTexture(*tex);
	mRectangle.width = tex->width;
	mRectangle.height = tex->height;
}

void HealingItem::onAcquired()
{
	static_cast<GamePlay*>(mSequence)->getPlayer()->
		getHpComp()->Recover(mHealAmount);

	SoundSystem::instance().playSE("HealSE");
}

SpeedUpItem::SpeedUpItem(Sequence* sequence)
	: ItemActor(sequence)
	, mDuration(15.0f)
	, mBuffMultiplier(1.5f)
{
	Texture2D* tex = mSequence->getTexture("Assets/SpeedUpItem.png");
	mSpriteComp->setTexture(*tex);
	mRectangle.width = tex->width;
	mRectangle.height = tex->height;
}

void SpeedUpItem::onAcquired()
{
	static_cast<GamePlay*>(mSequence)->getPlayer()
		->getPlayerMove()->setMultiplier(mBuffMultiplier, mDuration);
	SoundSystem::instance().playSE("HealSE");
}
