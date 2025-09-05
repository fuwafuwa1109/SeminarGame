#include "GamePlay.h"
#include "GameClear.h"
#include "GameOver.h"

#include <raylib.h>
#include <cmath>
#include <fstream>
#include <string>
#include <algorithm>
#include <iostream>

// Actor
#include "PlayerActor.h"
#include "EnemyActor.h"
#include "WeaponActor.h"
// Comopnent
#include "PlayerMove.h"
#include "EnemyMove.h"
#include "SpriteComponent.h"
#include "WeaponComponent.h"

GamePlay::GamePlay()
{
	// TODO: �e�L�X�g�t�@�C������}�b�v����
	if (!loadStage("stage0.txt")) {
		// �u���[�N�|�C���g�u��
	}
	// TODO:�����Ń}�b�v�̐���,Player�̐������s��
	mPlayer = new PlayerActor(this, Actor::Eplayer);
	new EnemyActor(this, Actor::Eenemy);
}

GamePlay::~GamePlay()
{
	// �ȉ���delete��Sequence�f�X�g���N�^��destroyEnemy�ł���Ă�̂ŕs�K�v
	/*while (!mEnemies.empty()) {
		delete mEnemies.back();
	}
	delete mPlayer;*/
}

void GamePlay::input()
{
	// test�p
	if (IsKeyPressed(KEY_ENTER)) {
		mNext = new GameClear();
	}
	else if (IsKeyPressed(KEY_RIGHT_SHIFT)) {
		mNext = new GameOver();
	}
	// �SActor��input���Ă�
	mUpdatingActors = true;
	for (Actor* actor : mActors) {
		actor->input();
	}
	mUpdatingActors = false;
}

void GamePlay::update()
{
	// Actor��update
	mUpdatingActors = true;
	for (Actor* actor : mActors) {
		actor->update();
	}
	mUpdatingActors = false;
	// �ۗ�����Actor��mActors�ֈړ�
	for (auto pending : mPendingActors)
	{
		mActors.emplace_back(pending);
	}
	mPendingActors.clear();

	// Collision��update
	// �S�A�N�^�[(�Ƃ���Component)��update��ɌĂ΂�Ă��邱�Ƃɒ���
	updateCollision();

	// Dead��Ԃ�Actor��delete
	for (auto actor : mActors)
	{
		if (actor->getState() == Actor::Edead)
		{
			delete actor;
		}
	}
}

void GamePlay::draw()
{
	BeginDrawing();
	ClearBackground(WHITE);

	// ui�̕`��
	DrawText("GamePlay", 100, 100, 40, BLACK);
	DrawText("Press ENTER -> GameClear", 100, 200, 20, GRAY);
	DrawText("Press RightShift -> GameOver", 100, 300, 20, GRAY);

	// �J�����Ɉڂ����̂̕`��(ui�ȊO)
	BeginMode2D(mPlayer->getCamera());
	for (auto& rec : mStageRecs)
	{
		DrawRectangleRec(rec, GRAY);
	}
	for (auto sprite : mSprites)
	{
		sprite->draw();
	}
	EndMode2D();

	EndDrawing();
}

void GamePlay::unloadData()
{
	Sequence::unloadData();
	while (!mActors.empty())
	{
		delete mActors.back();
	}
}

Sequence* GamePlay::nextSequence()
{
	return mNext;
}

bool GamePlay::loadStage(const char* filename)
{
	// �e�L�X�g�t�@�C������X�e�[�W��ǂݍ���
	// 0 : �����Ȃ��ꏊ
	// 1 : ���ƕ�
	// �ȈՓI�Ȃ��̂ł�
	// 1�̉������̘A�������rectangle�Ƃ��ĔF�������Ă���

	std::ifstream file(filename);
	std::string line;
	std::vector<std::vector<int>> tiles;
	
	// 2�����z��tiles�Ƀf�[�^���ڂ�
	while (std::getline(file, line))
	{
		std::vector<int> row;
		for (char c : line)
		{
			if (c == '0') row.push_back(0);
			else if (c == '1') row.push_back(1);
		}
		tiles.push_back(row);
	}
	// ��̃^�C���̏c���̒���
	const int tileSize = 32;
	mStageRecs.clear();
	mStageWidth = (int)tiles[0].size() * tileSize;
	mStageHeight = (int)tiles.size() * tileSize;

	for (int y = 0; y < (int)tiles.size(); ++y)
	{
		int startX = -1;
		for (int x = 0; x < (int)tiles[y].size(); ++x)
		{
			if (tiles[y][x] == 1)
			{
				// 1�̘A���̍��[���L�^����
				if (startX == -1) startX = x;
			}
			else
			{
				if (startX != -1)
				{
					// ��������1�̘A�����܂Ƃ߂�Rectangle��
					Rectangle r;
					r.x = startX * tileSize;
					r.y = y * tileSize;
					r.width = (x - startX) * tileSize;
					r.height = tileSize;
					mStageRecs.push_back(r);
					startX = -1;
				}
			}
		}
		// �s�̍Ō�ŘA�����Ă����ꍇ
		if (startX != -1)
		{
			Rectangle r;
			r.x = startX * tileSize;
			r.y = y * tileSize;
			r.width = (tiles[y].size() - startX) * tileSize;
			r.height = tileSize;
			mStageRecs.push_back(r);
		}
	}
	return true;
}

void GamePlay::addEnemy(EnemyActor* enemy)
{
	mEnemies.emplace_back(enemy);
}

void GamePlay::removeEnemy(EnemyActor* enemy)
{
	// mEnemies����폜
	auto iter = std::find(mEnemies.begin(), mEnemies.end(), enemy);
	if (iter != mEnemies.end()) {
		std::iter_swap(iter, mEnemies.end() - 1);
		mEnemies.pop_back();
	}
}

void GamePlay::addWeapon(WeaponActor* weapon, Actor::Type type)
{
	if (type == Actor::Eplayer) {
		mPlayerWeapons.emplace_back(weapon);
	}
	else if (type == Actor::Eenemy) {
		mEnemyWeapons.emplace_back(weapon);
	}
}

void GamePlay::removeWeapon(WeaponActor* weapon)
{
	auto iter = std::find(mPlayerWeapons.begin(), mPlayerWeapons.end(), weapon);
	if (iter != mPlayerWeapons.end()) {
		std::iter_swap(iter, mPlayerWeapons.end() - 1);
		mPlayerWeapons.pop_back();
	}
	else {
		iter = std::find(mEnemyWeapons.begin(), mEnemyWeapons.end(), weapon);
		if (iter != mEnemyWeapons.end()) {
			std::iter_swap(iter, mEnemyWeapons.end() - 1);
			mEnemyWeapons.pop_back();
		}
	}
}

void GamePlay::addSprite(SpriteComponent* sprite)
{
	// �\�[�g�ς݂̔z��ő}���_��������
	int myDrawOrder = sprite->getDrawOrder();
	auto iter = mSprites.begin();
	for (;
		iter != mSprites.end();
		++iter)
	{
		if (myDrawOrder < (*iter)->getDrawOrder())
		{
			break;
		}
	}
	// �C�e���[�^�̈ʒu�̑O�ɗv�f��}������
	mSprites.insert(iter, sprite);
}

void GamePlay::removeSprite(SpriteComponent* sprite)
{
	auto iter = std::find(mSprites.begin(), mSprites.end(), sprite);
	mSprites.erase(iter);
}

void GamePlay::updateCollision()
{
	Rectangle playerRec = mPlayer->getRectangle();

	// PlayerWeapon��Enemy�̏Փˌ��m,����
	for (auto enemy : mEnemies) {
		for (auto weapon : mPlayerWeapons)
		{
			Rectangle enemyRec = enemy->getRectangle();
			Rectangle weaponRec = weapon->getRectangle();
			if (CheckCollisionRecs(enemyRec, weaponRec)) {
				weapon->onHit(enemy);
			}
		}
	}

	// TODO:EnemyWeapon��Player�̏Փˌ��m,����
	for (auto weapon : mEnemyWeapons)
	{
		Rectangle playerRec = mPlayer->getRectangle();
		Rectangle weaponRec = weapon->getRectangle();
		if (CheckCollisionRecs(playerRec, weaponRec)) {
			// ����
			mNext = new GameOver();
		}
	}

	// TODO:�G��Player�̏Փˌ��m,����
	for (auto enemy : mEnemies) {
		if (CheckCollisionRecs(playerRec, enemy->getRectangle())) {
			// ����
			mNext = new GameOver();
		}
	}
	// Player�����łȂ�Enemy�����Ă�WeaponComponent�����Ηǂ��Ǝv��

	// Player���}�b�v�ƏՓ˂����Ƃ��̏���
	for (auto& stageRec : mStageRecs) {
		// �Փ˂��Ă���Ȃ�
		if (CheckCollisionRecs(playerRec, stageRec)) {
			// �Փ˕����̎l�p�`�𓾂�
			Rectangle colRec = GetCollisionRec(playerRec, stageRec);
			Vector2 playerPos = mPlayer->getPosition();
			// �c�����̏d�Ȃ�̕����������ꍇ��,�c�̏d�Ȃ肾������
			if (colRec.width >= colRec.height) {
				// player�ʒu����ɂ��炷
				if (playerRec.y < colRec.y) {
					playerPos.y -= colRec.height;
					//mPlayer->getPlayerMove().setJumping(false); // �W�����v��Ԃ�����
					mPlayer->getPlayerMove()->fixFloorCol();
				}
				// player�ʒu�����ɂ��炷
				else {
					playerPos.y += colRec.height;
				}
			}
			// �������̏d�Ȃ�̕����������ꍇ��,���̏d�Ȃ肾������
			else {
				// player�ʒu�����ɂ��炷
				if (playerRec.x < colRec.x) {
					playerPos.x -= colRec.width;
				}
				// player�ʒu���E�ɂ��炷
				else {
					playerPos.x += colRec.width;
				}
			}
			// �ʒu�̕ύX�𔽉f
			mPlayer->setPosition(playerPos);
			// Player�̎l�p�`�����炷
			mPlayer->computeRectangle();
			// playerRec�̍X�V���K�v
			playerRec = mPlayer->getRectangle();
		}
	}

	// Enemy���}�b�v�ƏՓ˂����Ƃ��̏���
	for (auto enemy : mEnemies) {
		Rectangle enemyRec = enemy->getRectangle();
		Vector2 enemyPos = enemy->getPosition();

		for (auto& stageRec : mStageRecs) {
			if (CheckCollisionRecs(enemyRec, stageRec)) {
				Rectangle colRec = GetCollisionRec(enemyRec, stageRec);
				//���̃W�����v�n������
				if (colRec.width < colRec.height) {
					// ���̏d�Ȃ����
					if (enemyRec.x < colRec.x) enemyPos.x -= colRec.width;
					else enemyPos.x += colRec.width;
					// �W�����v���Ȃ�ȍ~�̏����͕s�v
					if (enemy->getEnemyState() != EnemyActor::E_jump)
					{
						const int tileSize = 32;	
						int forward = enemy->getForward();
						bool isStep = false;
						if (enemyPos.x < stageRec.x && forward > 0 ||
							enemyPos.x > stageRec.x && forward < 0) {
							//�i�s�����̕ǂ�1�}�X�i�����`�F�b�N
							isStep = (stageRec.height <= tileSize * 1.5f);
						}
						//�P�}�X��A�P�}�X�オ���݂��邩�̊m�F�̎l�p�`��`
						Rectangle checkOneAbove = {
							colRec.x,
							stageRec.y - tileSize,
							colRec.width,
							1.0f
						};

						bool isSpaceAboveClear = true;
						for (const auto& otherStageRec : mStageRecs) {
							if (CheckCollisionRecs(checkOneAbove, otherStageRec)) {
								isSpaceAboveClear = false;
								break;
							}
						}

						if (isStep && isSpaceAboveClear) {
							enemy->jump();
							break;
						}
					}
				}
				//�c�����̏d�Ȃ肪�������ꍇ�A�c�̏d�Ȃ����
				else if (colRec.width >= colRec.height) {
					//��ɂ��炷
					if (enemyRec.y < colRec.y) {
						enemyPos.y -= colRec.height;
						enemy->getEnemyMove()->fixFloorCol();
					}
					//���ɂ��炷
					else {
						enemyPos.y += colRec.height;
					}
				}
				//enemy�̎l�p�`���Čv�Z
				enemy->setPosition(enemyPos);
				enemy->computeRectangle();
			}
		}
	}
}
