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

void SwordComponent::startAttack()
{
	mWeapon = new SwordActor(mOwner->getSequence(), Actor::Eweapon, mOwner->getType());
	mWeapon->onStartAttack();
	updateWeaponPosition();
	mWeapon->computeRectangle();
}

void SwordComponent::updateWeaponPosition()
{
	// ���폊�L�҂̑O���ɕ���̈ʒu��ݒ�
	Vector2 pos = mOwner->getPosition();
	mWeapon->setForward(mOwner->getForward());
	pos.x += (mOwner->getRectangle().width + mWeapon->getRectangle().width)
		* mWeapon->getForward() / 2.0f;
	mWeapon->setPosition(pos);
}
