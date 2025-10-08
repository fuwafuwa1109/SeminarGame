#include "SoundSystem.h"

SoundSystem* SoundSystem::sInstance = nullptr;

SoundSystem::SoundSystem()
    : mCurrentBGM(nullptr)
    , mBGMVolume(0.5f)
{
    InitAudioDevice();
}

SoundSystem::~SoundSystem()
{
    for (auto& kv : mSE) UnloadSound(kv.second);
    for (auto& kv : mBGM) UnloadMusicStream(kv.second);
    CloseAudioDevice();
}

void SoundSystem::loadSE(const std::string& name, const std::string& file)
{
    mSE[name] = LoadSound(file.c_str());
}

void SoundSystem::loadBGM(const std::string& name, const std::string& file)
{
    mBGM[name] = LoadMusicStream(file.c_str());
}

void SoundSystem::playSE(const std::string& name)
{
    if (mSE.count(name)) { 
        // BGMの音量を使っている
        SetSoundVolume(mSE[name], mBGMVolume);
        PlaySound(mSE[name]);
    }
}

void SoundSystem::playBGM(const std::string& name, bool loop)
{
    stopBGM();
    mCurrentBGM = &mBGM[name];
    PlayMusicStream(*mCurrentBGM);
    mCurrentBGM->looping = loop;
}

void SoundSystem::stopBGM()
{
    if (mCurrentBGM) {
        StopMusicStream(*mCurrentBGM);
        mCurrentBGM = nullptr;
    }
}

void SoundSystem::update()
{
    if (mCurrentBGM) {
        //音量調節
        if (IsKeyDown(KEY_UP)) {
            mBGMVolume += 0.01f;
        }
        if (IsKeyDown(KEY_DOWN)) {
            mBGMVolume -= 0.01f;
        }
        mBGMVolume = std::max(0.0f, std::min(mBGMVolume, 1.0f));
        //変更した音量をBGMに適用する
        SetMusicVolume(*mCurrentBGM, mBGMVolume);

        UpdateMusicStream(*mCurrentBGM);
    }
}
