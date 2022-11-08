#include"CPlayer2.h"
#include "CApplication.h"

#define TEXCOORD 168, 188, 158, 128 //テクスチャマッピング
#define GRAVITY (TIPSIZE / 25.0f)   //重力加速度
#define JUMPV0 (TIPSIZE / 1.35f)     //ジャンプの初速
#define TEXCRY 196, 216, 158, 128   //テクスチャマッピング
#define TEXCOORD2 136,156,158,128   //右向き2
#define TEXLEFT1 188,168,158,128    //左向き1
#define TEXLEFT2 156,136,158,128    //左向き2
#define SOUND_JUMP "res\\jump.wav"  //ジャンプSE
//#define VELOCITY 4.0f             //移動速度
//#define HP 3                      //HPの初期値は3

CPlayer2::CPlayer2(float x, float y, float w, float h, CTexture* pt)
	:mInvincible(0)
	,interval(0)
	//,mVy(0.0f)
{
	Set(x, y, w, h);
	Texture(pt, TEXCOORD);
	mTag = ETag::EPLAYER;
	sHp = 1;
	mSoundJump.Load(SOUND_JUMP);
	spInstance = this;
}

void CPlayer2::Update()
{
	if (interval > 0)
	{
		mState = EState::EJUMP;
		//ジャンプのインターバル
		interval--;
	}
	if (mInvincible > 0)
	{
		//無敵時間中は1減算する
		mInvincible--;
	}
	if (mInput.Key('A'))
	{
		mVx = -VELOCITY - 1;
		X(X() + mVx + mVx);
	}
	if (mInput.Key('D'))
	{
		mVx = VELOCITY + 1;
		X(X() + mVx + mVx);
	}
	if (mState != EState::EJUMP)
	{
		if (interval == 0)
		{
			if (mInput.Key('W'))
			{
				mVy = JUMPV0;
				mSoundJump.Play();
				mState = EState::EJUMP;
				interval = 35;
			}
		}
	}

	//Y座標にY軸速度を加える
	Y(Y() + mVy);
	//Y軸速度に重力を減算する
	mVy -= GRAVITY;
	{//通常の画像を設定
		Texture(Texture(), TEXCOORD);
	}

	if (mState == EState::ECRY)
	{
		//泣く画像を設定
		Texture(Texture(), TEXCRY);
		if (mInvincible == 0)
		{
			mInvincible = 60;
			sHp--;
		}
	}
	if (mInvincible > 0)
	{
		//泣く画像を設定
		Texture(Texture(), TEXCRY);
	}
	else
	{
		const int PITCH = 100;  //画像を切り替える間隔
		if ((int)X() % PITCH < PITCH / 2)
			if (mVx < 0.0f)  //左へ移動
			{
				//左向き1を設定
				Texture(Texture(), TEXLEFT1);
			}
			else
			{
				//通常の画像を設定
				Texture(Texture(), TEXCOORD);
			}
		else
			if (mVx < 0.0f) //左へ移動
			{
				//左向き2を設定
				Texture(Texture(), TEXLEFT2);
			}
			else
			{
				//2番目の画像を設定
				Texture(Texture(), TEXCOORD2);
			}
	}
}

void CPlayer2::Collision()
{
	CApplication::CharacterManager()->Collision(this);
}

void CPlayer2::Collision(CCharacter* m, CCharacter* o)
{
	float x, y;
	switch (o->Tag())
	{
	case ETag::EENEMY:
		if (CRectangle::Collision(o, &x, &y))
		{
			//敵の衝突判定を実行
			o->Collision(o, m);
			X(X() + x);
			Y(Y() + y);
			//着地した時
			if (y != 0.0f)
			{
				//Y軸速度を0にする
				mVy = 0.0f;
				if (y > 0.0f)
				{
					mState = EState::EJUMP;
					mVy = JUMPV0;
				}
				else
				{//ジャンプでなければ泣く
					mState = EState::ECRY;
				}
			}
			else
			{//ジャンプでなければ泣く
				mState = EState::ECRY;
			}
		}
		break;
	case ETag::EPLAYER:
		break;
	case ETag::EBLOCK:
		if (CRectangle::Collision(o, &x, &y))
		{
			X(X() + x);
			Y(Y() + y);
			//着地した時
			if (y != 0.0f)
			{
				//Y軸速度を0にする
				mVy = 0.0f;
					if (y > 0.0f)
					{
					mState = EState::EMOVE;
					}
			}
		}
		break;
	case ETag::ETRAP:
		if (CRectangle::Collision(o, &x, &y))
		{
			if (y > 0)
			{
				//mState = EState::EMOVE;
			}
		}
		break;
	case ETag::EWALL:
		if (CRectangle::Collision(o, &x, &y))
		{
			sHp--;
		}
		break;
	}
}

int CPlayer2::sHp = 0;  //HP

int CPlayer2::Hp()
{
	return sHp;
}

CPlayer2* CPlayer2::spInstance = nullptr;
CPlayer2* CPlayer2::Instance()
{
	return spInstance;
}
