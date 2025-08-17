#include "GamePlay.h"
#include "GameClear.h"
#include "GameOver.h"
#include "PlayerActor.h"
#include "EnemyActor.h"

#include <raylib.h>
#include <cmath>
#include <fstream>
#include <string>

GamePlay::GamePlay()
{
	// TODO: �e�L�X�g�t�@�C������}�b�v����
	if (!loadStage("stage0.txt")) {
		// �u���[�N�|�C���g�u��
	}
	// TODO:�����Ń}�b�v�̐���,Player�̐������s��
	mPlayer = new PlayerActor(this);
	new EnemyActor(this);
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
	for (Actor* actor : mActors) {
		actor->input();
	}
}

void GamePlay::update()
{
	// Actor��update
	for (Actor* actor : mActors) {
		actor->update();
	}
	// Collision��update
	updateCollision();
}

void GamePlay::draw()
{
	BeginDrawing();
	ClearBackground(WHITE);

	// text
	DrawText("GamePlay", 100, 100, 40, BLACK);
	DrawText("Press ENTER -> GameClear", 100, 200, 20, GRAY);
	DrawText("Press RightShift -> GameOver", 100, 300, 20, GRAY);

	// �J�����Ɉڂ����̂̕`��(ui�ȊO)
	BeginMode2D(mPlayer->getCamera());
	for (Actor* actor : mActors) {
		actor->draw();
	}
	for (auto rec : mStageRecs)
	{
		DrawRectangleRec(rec, GRAY);
	}
	EndMode2D();

	EndDrawing();
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
	
	// �󎟌��z��tiles�Ƀf�[�^���ڂ�
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

void GamePlay::destroyEnemy(EnemyActor* enemy)
{
	// mEnemies����폜
	auto iter = std::find(mEnemies.begin(), mEnemies.end(), enemy);
	if (iter != mEnemies.end()) mEnemies.erase(iter);
	// mActors����폜
	removeActor(enemy);
	delete enemy; // �Ō��delete
}

void GamePlay::updateCollision()
{
	// TODO:Weapon��Enemy�̏Փˌ��m,����
	for (auto enemy : mEnemies)
	{
		// destroyEnemy�������ŌĂ�
	}

	// TODO:�G��Player�̏Փˌ��m,����
	for (auto enemy : mEnemies)
	{
		if (CheckCollisionRecs(mPlayer->getRectangle(), enemy->getRectangle()))
		{
			//mNext = new GameOver();
			destroyEnemy(enemy);
		}
	}
	// Player�����łȂ�Enemy�����Ă�WeaponComponent�����Ηǂ��Ǝv��

	// TODO:Enemy,Player���}�b�v�ƏՓ˂����Ƃ��̏���
	
	// ��₱�������Ȃ�moveComponent������Ă����ɂ܂Ƃ߂鎖������
}
