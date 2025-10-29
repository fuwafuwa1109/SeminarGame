#include <raylib.h>
#include "Sequence.h"
#include "Title.h"
#include "SoundSystem.h"

const int WindowWidth = 1280;
const int WindowHeight = 720;

void LoadSounds();

int main()
{
	/* Window�̏����� */
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI); // ← 必ず InitWindow より前
	InitWindow(WindowWidth, WindowHeight, "SeminarProject");
	SetTargetFPS(60); // 60FPS
	SetWindowState(FLAG_VSYNC_HINT); // ��������ON

	// �T�E���h�V�X�e��������
	SoundSystem::init();
	// �T�E���h�A�Z�b�g���ꊇ���[�h
	LoadSounds();

	// �V�[�P���X������
	Sequence* currentSequence = new Title();

	// ���C�����[�v
	while (!WindowShouldClose())
	{
		/* ���� */
		currentSequence->input();
		
		/* �X�V */
		currentSequence->update();

		/* �T�E���h�X�V */
		SoundSystem::instance().update();

		/* �`�� */
		currentSequence->draw();
		DrawFPS(10, 20); // FPS��\�� �����Ă�����
	
		// �V�[�P���X�؂�ւ�
		Sequence* next = currentSequence->nextSequence();
		if (next != nullptr) {
			currentSequence->unloadData();
			delete currentSequence;
			currentSequence = next;
		}
	}
	
	currentSequence->unloadData();
	delete currentSequence;
	currentSequence = 0;

	SoundSystem::shutdown();

	CloseWindow();
	return 0;
}

// SoundSystem
void LoadSounds()
{
	SoundSystem& s = SoundSystem::instance();
	s.loadBGM("stageBGM", "Assets/uchinari_138.mp3");
	s.loadBGM("BossBGM", "Assets/shippuRaiden.mp3");

	s.loadSE("ItemSE", "Assets/Heal.mp3");
	s.loadSE("ExplosionSE", "Assets/Explosion2.mp3");
	s.loadSE("NormalAtkSE", "Assets/NormalAttack.mp3");
	s.loadSE("ChargeAtkSE", "Assets/ChargeAttack.mp3");
	s.loadSE("DodgeAtkSE", "Assets/DodgeAttack.mp3");
	s.loadSE("GuardSE", "Assets/Guard.mp3");
	s.loadSE("YarareSEa", "Assets/gu.mp3");
	s.loadSE("YarareSEb", "Assets/guaa.mp3");
}
