#include "AttackComponent.h"
#include <raymath.h>

#include "GamePlay.h"
#include "EnemyActor.h"
#include "PlayerActor.h"
#include "Boss.h"
#include "StageObject.h"

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
			
			// TODO: タスク1
			/* ------敵のノックバック関連の処理 開始------ */
			// 攻撃者(プレイヤー)の位置と攻撃対象の位置をキャッシュ
			// 必要に応じて使ってください
			Vector2 attackerPos = mOwner->getPosition();
			Vector2 targetPos = enemy->getPosition();
			
			KnockbackInfo info;
			// ★KnockbackInfoのパラメータを設定してください
			// ・定義はAttackComponent.hにあります
			// VisualStudioなら"KnockbackInfo"を右クリック->"定義をここに表示"で定義を見ることができます
			// ・ノックバック力はmCurInfo->で取得できます
			// ・上方向にも動かしたい場合,y座標は画面上部が0,下部が720という座標系であることに注意してください

			// TODO: 開催者: 以下を削除
			Vector2 direction = Vector2Normalize(Vector2Subtract(targetPos, attackerPos));
			float upspeed = 2.0f;
			direction.y -= upspeed;
			direction = Vector2Normalize(direction);

			info.target = enemy; // Knockback構造体のtargetにenemyを設定
			info.timer = 0.5f;
			float speed = mCurInfo->knockBack;
			info.velocity = Vector2Scale(direction, speed);

			// mKnockbackTargetsに追加
			// infoの設定ができたら下3行をアンコメントしてください
			// TODO: 開催者: 下行をコメントアウト
			if (enemy->getType() != EnemyActor::Type::Boss) {
				mKnockbackTargets.push_back(info);
			}

			/* ------敵のノックバック関連の処理 終了------ */

			if (auto boss = dynamic_cast<Boss*>(enemy)) {
				// TODO Boss にダメージを与えるロジックを実装する
				boss->ApplyDamage(mCurInfo->damage, mCurInfo->tag);
				// プレイヤーが倒した場合は Boss 撃破 / HP ゼロ で遷移するので setState を呼ぶ必要あり
			}
			else if (enemy->getHpComp()->TakeDamage(mCurInfo->damage)) {
				enemy->setState(Actor::Edead);
			}		
			mActive = false;
		}
	}

	// 敵に食らうダメージはオブジェクトにも食らう理屈
	std::vector<StageObject*> objs =
		static_cast<GamePlay*>(mOwner->getSequence())->getStageObjs();
	for (auto obj : objs) {
		if (obj->getType() == StageObject::Type::Ebreakable) {
			int i = 1;
			if (CheckCollisionRecs(obj->getRectangle(), mCurInfo->colRect)) {
				if (obj->getHpComp()->TakeDamage(mCurInfo->damage)) {
					obj->setState(Actor::Edead);
				}
				mActive = false;
			}
		}
	}
}

void AttackComponent::processAttackPlayer()
{
	PlayerActor* player = static_cast<GamePlay*>(mOwner->getSequence())->getPlayer();
	PlayerState* playerState = player->getPlayerState();

	// 無敵or回避ならダメージは与えない
	if (player->getHpComp()->isInvincible() ||
		playerState->getType() == PlayerState::Type::Dodge ||
		playerState->getType() == PlayerState::Type::DodgeAttack) {
		return;
	}

	// 当たったら
	if (CheckCollisionRecs(player->getRectangle(), mCurInfo->colRect)) {
		mHitActors.push_back(player);
		
		// ガード中なら
		if (playerState->getType() == PlayerState::Type::Guard) {
			static_cast<Guard*>(playerState)->onAttacked();
			// ダメージを食らわない
			// ダメージ半減等にするなら,returnを消してTakeDamageに倍率を掛ける
			return;
		}

		//　以下,ダメージやノックバックの処理

		Vector2 attackerPos = mOwner->getPosition();
		Vector2 targetPos = player->getPosition();
		Vector2 direction = Vector2Normalize(Vector2Subtract(targetPos, attackerPos));

		float flyspeed = 0.2f;
		direction.y -= flyspeed;
		direction = Vector2Normalize(direction);
		KnockbackInfo info;
		info.target = player;
		info.timer = 0.2f;
		float speed = mCurInfo->knockBack;
		info.velocity = Vector2Scale(direction, speed);

		mKnockbackTargets.push_back(info);

		player->getHpComp()->TakeDamage(mCurInfo->damage);
	}
}
