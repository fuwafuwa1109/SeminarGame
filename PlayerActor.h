#pragma once
#include "Actor.h"
#include <map>
#include "PlayerControl.h"

/// <summary>
/// PlayerActor�N���X
/// </summary>
class PlayerActor :
    public Actor
{
public:

    PlayerActor(class Sequence* sequence);

    void input() override;
    void update() override;
    void draw() override;

    const Camera2D& getCamera() const ;
    class PlayerControl& getPlayerControl();
    void computeRectangle();
    void setMoveTexture(PlayerControl::MoveState s);

private:
    class CameraComponent* mCameraComp;
    class PlayerControl* mPlayerControl;
    std::map<PlayerControl::MoveState, Texture2D> mMoveTextures;
};

// ����
// AD : �ړ�
// Space : �W�����v
// Shift + �ړ� : �_�b�V��
// S : �K�[�h(�ړ�,�W�����v�s��)
// W : �U��

// ��� ��Ԃɂ���ăA�j���[�V������ς���
// �U��...1���̊G
// �ړ�...���[�v�A�j���[�V����
// �W�����v...�W�����v�̊G
// �K�[�h...�ꖇ�̊G
// ���Ă����(��U�ۗ�)