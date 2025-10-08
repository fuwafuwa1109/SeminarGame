#pragma once
#include <vector>
#include <raylib.h>

class Stage
{
public:
    Stage(class GamePlay* sequence);
    ~Stage();
	void loadStage(const char* filename);
    void update();

    int getStageWidth() const { return mStageWidth; }

    // 
    std::vector<struct Rectangle> getStageRecs() const;

private:
    class GamePlay* mGamePlay;
    class EnemySpawner* mSpawner;

    // ȉ̓Xe[WL̏
    // Xe[Wǂݍ񂾎lp`̔z(ǂ̕`,collisionɎgp)
    int mStageWidth;
    int mStageHeight;
    struct Rectangle BossEntrance{0,0,0,0};  // {XGAւ̓
    std::vector<Vector2> mEnemySpawnPoints;
};

