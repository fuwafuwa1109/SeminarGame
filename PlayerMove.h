#pragma once
#include "MoveComponent.h"

/// <summary>
/// Player‚ÌˆÚ“®‚ği‚é
/// Player‚ÌState‚ğ•ÏX‚µ‚½‚è‚à‚·‚é
/// </summary>
class PlayerMove :
    public MoveComponent
{
public:
    PlayerMove(class PlayerActor* owner);
    void input()override;
    void update()override;
    void fixFloorCol();
    void fixCeilingCol();

    // ƒWƒƒƒ“ƒv‚·‚éuŠÔ‚ÉŒÄ‚Ô
    void jump();

private:
    class PlayerActor* mPlayer; // mOwner‚Æ“¯‚¶(Œ^‚ªˆá‚¤)
};
