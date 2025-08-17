#pragma once
#include "Actor.h"

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

    Camera2D& const getCamera();

private:
    class CameraComponent* mCameraComp;
};

