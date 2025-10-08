#include "Stage.h"
#include <fstream>
#include <string>
#include <vector>
#include <raymath.h>

#include "GamePlay.h"
#include "PlayerActor.h"
#include "EnemySpawner.h"
#include "StageObject.h"

Stage::Stage(GamePlay* sequence)
    : mGamePlay(sequence)
    , mStageHeight(0)
    , mStageWidth(0)
{
    mSpawner = new EnemySpawner(sequence);
}

Stage::~Stage()
{
    delete mSpawner;
    mSpawner = nullptr;
}

void Stage::loadStage(const char* filename)
{
    BossEntrance = Rectangle{0,0,0,0};
// Xe[Wt@C̓ǂݍ
    // ' ' : (ʍs\)
    // # : ubN()
    // w : ؂̔
    // E : G̏oʒu
    // B : {XGA̓ ɃvC[ڐG{XXe[W̓ǂݍ݂n߂
    // ͂Ƃ肠VvȎ

    std::ifstream file(filename);
    std::string line;
    std::vector<std::vector<char>> tiles;

    // 2ztilesɓǂݍ
    while (std::getline(file, line))
    {
        std::vector<char> row;
        for (char c : line)
        {
            row.push_back(c);
        }
        tiles.push_back(row);
    }

    // Xe[WŜ̕EvZ
    const int tileSize = 40;
    mStageWidth = (int)tiles[0].size() * tileSize;
    mStageHeight = (int)tiles.size() * tileSize;

    for (int y = 0; y < (int)tiles.size(); ++y)
    {
        int startX = -1;
        for (int x = 0; x < (int)tiles[y].size(); ++x)
        {
            // ̏ꍇ
            if (tiles[y][x] == '#')
            {
                // 1n܂L^
                if (startX == -1) startX = x;
            }
            else
            {
                // ȂȂrectangle
                if (startX != -1)
                {
                    // 1r؂ꂽRectangleɕϊ
                    Rectangle r;
                    r.x = (float)startX * tileSize;
                    r.y = (float)y * tileSize;
                    r.width = (float)(x - startX) * tileSize;
                    r.height = (float)tileSize;

                    int tileNum = x - startX;
                    HardObj* obj = new HardObj(mGamePlay, tileNum, r);
                    startX = -1;
                }
                // Goʒȕꍇ
                if (tiles[y][x] == 'E')
                {
                    Vector2 pos = { (float)x * tileSize + tileSize / 2.0f, (float)y * tileSize };
                    mEnemySpawnPoints.push_back(pos);
                }
                else if (tiles[y][x] == 'W')
                {
                    Rectangle r;
                    r.x = (float)x * tileSize;
                    r.y = (float)y * tileSize;
                    r.width = (float)tileSize;
                    r.height = (float)tileSize;
                    BreakableObj* bo = new BreakableObj(mGamePlay, r);
                }
                else if (tiles[y][x] == 'B')
                {
                    // AN^[ɂĂ
                    BossEntrance.x = (float)x * tileSize;
                    BossEntrance.y = (float)y * tileSize;
                    BossEntrance.width = tileSize;
                    BossEntrance.height = tileSize;
                }
            }
        }
        // s܂1Ăꍇ
        if (startX != -1)
        {
            Rectangle r;
            r.x = (float)startX * tileSize;
            r.y = (float)y * tileSize;
            r.width = (float)(tiles[y].size() - startX) * tileSize;
            r.height = (float)tileSize;

            int tileNum = tiles[y].size() - startX;
            HardObj* obj = new HardObj(mGamePlay, tileNum, r);
            startX = -1;
        }
    }
}

void Stage::update()
{
    // vC[ƃX|[n_̋vZāA߂ÂGo
    PlayerActor* player = mGamePlay->getPlayer();

    Vector2 playerPos = player->getPosition();

    auto iter = mEnemySpawnPoints.begin();
    while (iter != mEnemySpawnPoints.end()) {
        float dist = Vector2Distance(playerPos, *iter);
        // XN[̔ȉ̋ɂȂoɂĂ݂
        if (dist < GetScreenWidth() / 2.0f + 10.0f) {
            mSpawner->spawnAt(*iter);
            iter = mEnemySpawnPoints.erase(iter);
        }
        else {
            ++iter;
        }
    }

    // vC[{XGA̓ɐN
    if (CheckCollisionRecs(player->getRectangle(), BossEntrance)) {
        BossEntrance.width = 0.0f;
        BossEntrance.height = 0.0f;
        BossEntrance.x = 0.0f;
        BossEntrance.y = 0.0f;
        mGamePlay->onEnterBossArea();
    }
}

std::vector<struct Rectangle> Stage::getStageRecs() const
{
    std::vector<struct Rectangle> ret;
    std::vector<StageObject*> objs = mGamePlay->getStageObjs();
    ret.reserve(objs.size());
    for (auto* o : objs) {
        ret.push_back(o->getRectangle());
    }
    return ret;
}
