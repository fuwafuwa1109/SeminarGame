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
    // �s���̏�ԂƃA�N�V�����̏�Ԃɕ�����
    // ���ꂼ��̏�Ԃ�\���ϐ�������(mMoveState,mActionState)
    // enum�𕪂��Ă��Ȃ���,mActionState = ms_idle �͉\,�C��t����
    // �X�e�[�g���̏�����������,enum�ł͂Ȃ�class�ɂ������������Ǝv��
    // �N���X�ɂ���΂������Ƀe�N�X�`��������������,AttackTime�̂悤��,�����Ԃł���
    // �g��Ȃ��ϐ��������Ԃ݂̂ɉ������߂邱�Ƃ��ł���
    enum PlayerState
    {
        // moveState
        ms_idle,
        ms_walk,
        ms_jump,    // �W�����v�Ƃ��������󒆂ɂ���X�e�[�g�Ƃ�������
        // actionState
        as_idle,
        as_attack,
        as_guard,
        as_charge,
        as_dodge, // ���
    };
    PlayerActor(class Sequence* sequence, Type type);

    void input() override;
    void update() override;

    const Camera2D& getCamera() const ;
    class PlayerControl& getPlayerControl();
    class SwordComponent* getWeapon() const { return mSwordComp; }
    PlayerState getMoveState() { return mMoveState; }
    PlayerState getActionState() { return mActionState; }
    
    void computeRectangle() override;
    void changeState(PlayerState nextState);

private:
    void onEnterState(PlayerState nextState);
    void onExitState(PlayerState nextState);
    
    //std::vector<Texture2D*>
    PlayerState mMoveState;
    PlayerState mActionState;
    class CameraComponent* mCameraComp;
    class PlayerControl* mPlayerControl;
    class AnimSpriteComponent* mAnimsc;
    class SwordComponent* mSwordComp;
    class ArrowComponent* mArrowComp;
};

// ����
// AD : �ړ�
// Space : �W�����v
// Shift + �ړ� : �_�b�V��
// S : �K�[�h(�ړ�,�W�����v�s��)
// W : �U��
