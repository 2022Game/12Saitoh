#include "CMagicBullet.h"
#include "CPlayer.h"
#include "CApplication.h"

#define MAGICBULLET 0, 200, 1400, 1200
#define MAGICBULLET2 200, 0, 1400, 1200 

//コンストラクタ
CMagicBullet::CMagicBullet(float x, float y, float w, float h, CTexture* pt)
	:CCharacter((int)ETaskPriority::ECharacter)
	, mDc(0)
{
	Set(x, y, w, h );
	Texture(pt, MAGICBULLET);
	mTag = ETag::EBULLET;
	mState = EState::ENULL;
	mDc = 20;
}

//デストラクタ
CMagicBullet::~CMagicBullet()
{
	/*Enabled = false;*/
}

void CMagicBullet::Collision(CCharacter* m, CCharacter* o)
{
	////めり込み調整変数を宣言する
	switch (o->Tag())
	{
	case ETag::EENEMY:
		if (CRectangle::Collision(o))
		{ 
			if (CApplication::Bd() == 1)
			{
				/*mEnabled = false;*/
			}
		}
		break;
	}
}
//更新処理
void CMagicBullet::Update()
{
	mDc--;

	if (CApplication::Di() == 1)
	{
		mDc = 0;
	}
	if (mDc <= 0)
	{
		mEnabled = false;
	}
	if (CPlayer::Instance()->mVx >=  0)
	{
		mVx = VELOCITY + 10;
		X(X() + mVx);
	}
	else
	{
		Texture (Texture(),MAGICBULLET2);
		mVx = -VELOCITY - 10;
		X(X() + mVx);
	}
	CCharacter::Update();
}