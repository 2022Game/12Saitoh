#include "CMagicBullet.h"
#include "CPlayer.h"
#include "CApplication.h"

#define MAGICBULLET 0, 200, 1400, 1200
#define MAGICBULLET2 200, 0, 1400, 1200 

//�R���X�g���N�^
CMagicBullet::CMagicBullet(float x, float y, float w, float h, CTexture* pt)
	:CCharacter((int)ETaskPriority::ECharacter)
{
	Set(x, y, w, h );
	Texture(pt, MAGICBULLET);
	mTag = ETag::EBULLET;
	mState = EState::ENULL;
}

//�f�X�g���N�^
CMagicBullet::~CMagicBullet()
{
}

//�Փˏ���
void CMagicBullet::Collision(CCharacter* m, CCharacter* o)
{
	////�߂荞�ݒ����ϐ���錾����
	switch (o->Tag())
	{
	case ETag::EENEMY:
		if (CRectangle::Collision(o))
		{ 
		}
		break;
	}
}

//�X�V����
void CMagicBullet::Update()
{
	if (CPlayer::Instance()->mVx >=  0)
	{
		mVx = VELOCITY + 10;
		X(X() + mVx);
		mAnimationCount++;
	}
	else
	{
		Texture (Texture(),MAGICBULLET2);
		mVx = -VELOCITY - 10;
		X(X() + mVx);
		mAnimationCount++;
	}
	if (mAnimationCount > 20)
	{
		mEnabled = false;
	}
	CCharacter::Update();
}