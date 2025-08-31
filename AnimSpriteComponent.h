#pragma once
#include "SpriteComponent.h"
#include <vector>
#include <string>
#include <unordered_map>
#include <raylib.h>

/// <summary>
/// �A�j���[�V�������
/// </summary>
struct Animation
{
    std::string name;                  // �A�j���[�V������
    std::vector<Texture2D*> frames;    // �e�t���[���̃e�N�X�`��
    float fps;                         // �Đ�FPS
    bool loop;                         // ���[�v�A�j���[�V�������ǂ���
};

/// <summary>
/// �A�j���[�V�����t���X�v���C�g�̕`��
/// </summary>
class AnimSpriteComponent :
    public SpriteComponent
{
public:
    AnimSpriteComponent(class Actor* owner, int drawOrder = 100);
    // ���̃t���[�����ƂɃA�j���[�V�������X�V component����override
    void update() override;
    // �A�j���[�V�����Ɏg���e�N�X�`���̐ݒ�
    void setAnimTextures(std::vector<Texture2D*> textures);
    void setTexturesMap(std::string name, std::vector<Texture2D*> textures);
    // �A�j���[�V�������Đ�����
    void play(int begin, int end, bool loop, float fps = 60.0f);

    // �A�j���[�V������ǉ�
    void addAnimation(const std::string& name,
        const std::vector<Texture2D*>& frames,
        float fps = 60.0f,
        bool loop = true);
    // �A�j���[�V�������Đ�����
    void play(std::string animName);

    bool isAnimating() const { return mIsAnimating; }

private:
    std::unordered_map<std::string, Animation> mAnimations;
    Animation* mCurrentAnim;  // ���݂̃A�j���[�V����
    // �A�j���[�V�����ł̂��ׂẴe�N�X�`��
    std::vector<Texture2D*> mAnimTextures;
    std::unordered_map<std::string, std::vector<Texture2D*>> mTexturesMap;
    // ���ݕ\�����Ă���t���[��
    float mCurrFrame;
    // �A�j���[�V������FPS ���I�ɕύX�\
    float mAnimFPS;
    bool mLoopFlag;
    int mAnimBegin;
    int mAnimEnd;
    int mFrameCount;
    bool mIsAnimating;
};