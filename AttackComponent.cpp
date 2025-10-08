#include "AttackComponent.h"
#include <raymath.h>

#include "GamePlay.h"
#include "EnemyActor.h"
#include "PlayerActor.h"
#include "Boss.h"

#include "HpComponent.h"

AttackComponent::AttackComponent(Actor* owner)
	: Component(owner)
	, mTimer(0.0f)
	, mActive(false)
	, mCurInfo(nullptr)
{
}

void AttackComponent::update()
{
	if (mActive) {
		/* 攻撃終了判定 */
		mTimer += GetFrameTime();
		// 攻撃時間を過ぎたら非activeに
		if (mTimer > mCurInfo->duration) {
			mActive = false;
		}

		switch (mCurInfo->targetType)
		{
			// 敵への攻撃の場合
		case Actor::Type::Eenemy:
			processAttackEnemy();
			break;
		case Actor::Type::Eplayer:
			processAttackPlayer();
			break;
		}
	}
	//kb move
	for (int i = mKnockbackTargets.size() - 1; i >= 0; --i) {
		auto& info = mKnockbackTargets[i];

		//ターゲットが有効かチェック
		if (info.target->getState() == Actor::Edead) {
			mKnockbackTargets.erase(mKnockbackTargets.begin() + i);
			continue;
		}
		info.timer -= GetFrameTime();

		if (info.timer <= 0.0f) {
			mKnockbackTargets.erase(mKnockbackTargets.begin() + i);
		}
		else {
			// TODO: タスク2
			/* ------ ノックバック中の位置更新 開始------ */
			Vector2 currentPos = info.target->getPosition();
			info.velocity.y += 50.0f;
			Vector2 moveAmount = Vector2Scale(info.velocity, GetFrameTime());
			Vector2 newPos = Vector2Add(currentPos, moveAmount);

			// NOTE: 以下弄らなくてよい
			info.target->setPosition(newPos);
			info.target->computeRectangle();
			/* ------ 敵のノックバック中の位置更新 終了------ */
		}
	}
}

void AttackComponent::startAttack(AttackInfo* info)
{
	mActive = true;
	mTimer = 0.0f;
	mCurInfo = info; // 現在の攻撃を設定
	mHitActors.clear();
}

void AttackComponent::processAttackEnemy()
{
	std::vector<EnemyActor*> enemies = 
		static_cast<GamePlay*>(mOwner->getSequence())->getEnemies();
	for (auto enemy : enemies) {

		if (enemy->getHpComp()->isInvincible()) {
			continue;
		}
		if (CheckCollisionRecs(enemy->getRectangle(), mCurInfo->colRect)) {
			mHitActors.push_back(enemy);
			//Kb
			Vector2 attackerPos = mOwner->getPosition();
			Vector2 targetPos = enemy->getPosition();
			Vector2 direction = Vector2Normalize(Vector2Subtract(targetPos, attackerPos));

			float upspeed = 2.0f;
			direction.y -= upspeed;
			direction = Vector2Normalize(direction);

			KnockbackInfo info;
			info.target = enemy; //Knockback構造体のtargetにenemyを設定
			info.timer = 0.2f;
			float speed = 300.0f;
			info.velocity = Vector2Scale(direction, speed);

			mKnockbackTargets.push_back(info);

			if (enemy->getHpComp()->TakeDamage(mCurInfo->damage)) {
				enemy->setState(Actor::Edead);
				mActive = false;
			}
		}
		
    // コンフリクト解消の都合で,とりあえず
		if (CheckCollisionRecs(enemy->getRectangle(), mCurInfo->colRect)) {
			if (auto boss = dynamic_cast<Boss*>(enemy)) {
				// Boss クラスにダメージを与える処理
				boss->ApplyDamage(mCurInfo->damage, mCurInfo->tag);
				// もしBossが倒れた場合/HPが0になった場合はsetStateを呼ぶ
			}
		}

	}
}

void AttackComponent::processAttackPlayer()
{
	PlayerActor* player = static_cast<GamePlay*>(mOwner->getSequence())->getPlayer();

	if (player->getHpComp()->isInvincible()) {
		return;
	}
	// ダメージ与える
	if (CheckCollisionRecs(player->getRectangle(), mCurInfo->colRect)) {
		mHitActors.push_back(player);
		Vector2 attackerPos = mOwner->getPosition();
		Vector2 targetPos = player->getPosition();
		Vector2 direction = Vector2Normalize(Vector2Subtract(targetPos, attackerPos));

		float flyspeed = 0.2f;
		direction.y -= flyspeed;
		direction = Vector2Normalize(direction);
		KnockbackInfo info;
		info.target = player;
		info.timer = 0.2f;
		float speed = 150.0f;
		info.velocity = Vector2Scale(direction, speed);

		mKnockbackTargets.push_back(info);

		player->getHpComp()->TakeDamage(mCurInfo->damage);
	}
}
