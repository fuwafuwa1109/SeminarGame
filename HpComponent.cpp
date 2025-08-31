#include "HpComponent.h"
#include <iostream>

HpComponent::HpComponent(Actor* owner, float maxHp)
	: Component(owner)
	, mMaxHp(maxHp)
	, mCurHp(maxHp)
{
}

bool HpComponent::TakeDamage(float damage)
{
	// hp��0�ȉ��ɂȂ�Ȃ�
	mCurHp -= damage;
	if (mCurHp <= 0.0f) {
		mCurHp = 0.0f;
		return true;
	}
	//�f�o�b�O�p
	std::cout << "mCurHp=" << mCurHp << std::endl;
	return false;
}

void HpComponent::Recover(float recover)
{
	// hp��mMaxHp�𒴂��Ȃ�
	float diff = mMaxHp - mCurHp;
	if (diff <= recover) {
		recover = diff;
	}
	mCurHp += recover;
}

void HpComponent::AddMaxHp(float add)
{
	mMaxHp += add;
	mCurHp += add;
}
