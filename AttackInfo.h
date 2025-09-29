#pragma once
#include <raylib.h>
#include <vector>
#include "Actor.h"
#include "DamageTags.h"

struct AttackInfo
{
	float damage;		// ダメージ量
	float duration;		// 持続
	Rectangle colRect;	// 攻撃範囲
	float knockBack;	// ノックバック力

	Actor::Type targetType;   // 攻撃対象
	DamageTag tag = DamageTag::Normal; 
};