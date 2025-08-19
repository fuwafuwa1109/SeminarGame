#pragma once
#include <raylib.h>
#include <vector>

/// <summary>
/// �Q�[���I�u�W�F�N�g�S�ʂ̊��N���X
/// </summary>
class Actor
{
public:	
	Actor(class Sequence* sequenece);
	virtual ~Actor();
	
	virtual void input();
	virtual void update();
	virtual void draw();

	void addComponent(class Component* component);

	virtual const Rectangle& getRectangle() const { return mRectangle; }
	const Vector2& getPosition() const { return mPosition; }
	Sequence* getSequence() const { return mSequence; }

	void setPosition(Vector2 pos) { mPosition = pos; }

protected:
	Sequence* mSequence;
	Texture2D mTexture;	  //
	Vector2 mPosition;	  // �e�N�X�`���̒��S���W
	// �����蔻��p�̋�`,�ۂƂ��F�X�g���Ȃ�h���N���X�Ɏ����Ă���
	Rectangle mRectangle;
	std::vector<class Component*> mComponents;
};

