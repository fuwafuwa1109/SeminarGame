#include "AnimSpriteComponent.h"
#include <iostream>

AnimSpriteComponent::AnimSpriteComponent(Actor* owner, int drawOrder)
	: SpriteComponent(owner, drawOrder)
	, mCurrFrame(0.0f)
	, mIsAnimating(false)
	, mCurrentAnim(nullptr)
{
}

void AnimSpriteComponent::update()
{
	SpriteComponent::update();

	if (mCurrentAnim && mIsAnimating)
	{
		// �t���[����i�߂Ă���
		mCurrFrame += mCurrentAnim->fps / GetFPS();
		// �t���[�����𒴂�����
		if (mCurrFrame >= (int)mCurrentAnim->frames.size())
		{
			// ���[�v�A�j���[�V�����̏ꍇ
			if (mCurrentAnim->loop) {
				// �ŏ��̃t���[���ɕ␳�t���Ŗ߂�
				mCurrFrame = fmod(mCurrFrame, (float)mCurrentAnim->frames.size());
			}
			// ���[�v���Ȃ��ꍇ
			else {
				// �ŏI�t���[���ɐݒ�
				mCurrFrame = (float)(mCurrentAnim->frames.size() - 1);
				// �A�j���[�V�����I��
				mIsAnimating = false;
			}
		}
		// �\���e�N�X�`���X�V
		setTexture(*mCurrentAnim->frames[(int)mCurrFrame]);
	}
}

void AnimSpriteComponent::addAnimation(const std::string& name, const std::vector<Texture2D*>& frames, float fps, bool loop)
{
	// �A�j���[�V�������쐬���A���e��ݒ�
	Animation anim;
	anim.name = name;
	anim.frames = frames;
	anim.fps = fps;
	anim.loop = loop;
	// �}�b�v�ɒǉ�
	mAnimations[name] = anim;
}

void AnimSpriteComponent::play(std::string animName)
{
	auto it = mAnimations.find(animName);
	if (it != mAnimations.end()) {
		mCurrentAnim = &it->second;
		mCurrFrame = 0.0f;
		mIsAnimating = true;
		setTexture(*mCurrentAnim->frames[0]);
	}
}
