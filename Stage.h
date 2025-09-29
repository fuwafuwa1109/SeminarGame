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

    // 正直無くしたい
    std::vector<struct Rectangle> getStageRecs() const;

private:
    class GamePlay* mGamePlay;
    class EnemySpawner* mSpawner;

    // 以下はステージ特有の情報
    // ステージから読み込んだ四角形の配列(床や壁の描画,collisionに使用)
    int mStageWidth;
    int mStageHeight;
    struct Rectangle BossEntrance;  // ボスエリアへの入り口
    std::vector<Vector2> mEnemySpawnPoints;
};

