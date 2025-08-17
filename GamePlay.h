#pragma once
#include "Sequence.h"

/// <summary>
/// GamePlay�V�[�P���X
/// </summary>
class GamePlay :
    public Sequence
{
public:
    GamePlay();
    ~GamePlay() override;

    void input() override;
    void update() override;
    void draw() override;
    Sequence* nextSequence() override;

    bool loadStage(const char* filename);

    void addEnemy(class EnemyActor* enemy);
    std::vector<class EnemyActor*> const getEnemies() { return mEnemies; }

    int& const getStageWidth() { return mStageWidth; }

private:
    void destroyEnemy(class EnemyActor* enemy);
    // collision�֘A�̍X�V�������܂Ƃ܂��Ă�
    void updateCollision();
    Sequence* mNext = nullptr;
    // �G�̔z��(���Collision�p�Ɏg��)
    std::vector<class EnemyActor*> mEnemies;
    class PlayerActor* mPlayer;
    // �X�e�[�W����ǂݍ��񂾎l�p�`�̔z��(�����)
    std::vector<class Rectangle> mStageRecs;
    int mStageWidth;
    int mStageHeight;
    Camera2D mCamera;
};

// collision�̏�����collisionManager�݂����Ȃ̍����
// �O�ɏo���Ă����������ł�
