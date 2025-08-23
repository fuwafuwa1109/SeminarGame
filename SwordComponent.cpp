#include "SwordComponent.h"
#include "SwordActor.h"

SwordComponent::SwordComponent(Actor* owner)
	: WeaponComponent(owner)
{
}

void SwordComponent::update()
{
	if (mWeapon) {
		updateWeaponPosition();
	}
}

void SwordComponent::startAttack(int begin, int end, float attackTime)
{
	mWeapon = new SwordActor(mOwner->getSequence(), Actor::Eweapon, mOwner->getType());
	mWeapon->onStartAttack(begin, end, attackTime);
	updateWeaponPosition();
	mWeapon->computeRectangle();
}

void SwordComponent::updateWeaponPosition()
{
	// ���폊�L�҂̑O���ɕ���̈ʒu��ݒ�
	Vector2 pos = mOwner->getPosition();
	mWeapon->setForward(mOwner->Forward());
	pos.x += (mOwner->getRectangle().width + mWeapon->getRectangle().width)
		* mWeapon->Forward() / 2.0f;
	mWeapon->setPosition(pos);
}
