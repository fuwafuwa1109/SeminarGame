#include "AnimSpriteComponent.h"
#include <iostream>

AnimSpriteComponent::AnimSpriteComponent(Actor* owner, int drawOrder)
	: SpriteComponent(owner, drawOrder)
	, mCurrFrame(0.0f)
	, mAnimFPS(30.0f)
	, mLoopFlag(true)
	, mAnimBegin(0)
	, mAnimEnd(1)
	, mFrameCount(1)
	, mIsAnimating(false)
{
}

void AnimSpriteComponent::update()
{
	SpriteComponent::update();

	// �\������摜��ݒ肷��
	if (mAnimTextures.size() > 0)
	{
		// ���[�v�A�j���[�V�����̏ꍇ
		if (mLoopFlag)
		{
			mCurrFrame += mAnimFPS / (float)GetFPS();
			// �ŏI�t���[���܂ōs������,�␳���čŏ�����
			if (mCurrFrame >= mFrameCount) {
				mCurrFrame = fmod(mCurrFrame, (float)mFrameCount);
			}
			setTexture(*mAnimTextures[static_cast<int>(mCurrFrame + mAnimBegin)]);
		}
		// ���[�v�ȊO�̏ꍇ
		else if (!mLoopFlag && mIsAnimating)
		{
			mCurrFrame += mAnimFPS / (float)GetFPS();
			// �A�j���[�V�����I�����̏���
			if (mCurrFrame >= mFrameCount)
			{
				mCurrFrame = 0.0f;
				mIsAnimating = false;
			}
			setTexture(*mAnimTextures[static_cast<int>(mCurrFrame + mAnimBegin)]);
		}
		// ���[�v�łȂ��A�j���[�V�����̏I�����A�擪�̃e�N�X�`���ɐݒ肷��
		else if (!mLoopFlag && !mIsAnimating)
		{
			setTexture(*mAnimTextures[0]);
		}
	}
}

void AnimSpriteComponent::setAnimTextures(std::vector<Texture2D*> textures)
{
	mAnimTextures = textures;
	if (mAnimTextures.size() > 0)
	{
		// �擪�̃e�N�X�`����ݒ�
		mCurrFrame = 0.0f;
		setTexture(*mAnimTextures[0]);
	}
}

void AnimSpriteComponent::play(int begin, int end, bool loop, float fps)
{
	mAnimBegin = begin;
	mAnimEnd = end;
	mFrameCount = end - begin + 1;
	mAnimFPS = fps;
	mLoopFlag = loop;
	mCurrFrame = 0.0f;
	mIsAnimating = true;

	// �J�n���̃t���[����ݒ�
	setTexture(*mAnimTextures[mAnimBegin]);
}