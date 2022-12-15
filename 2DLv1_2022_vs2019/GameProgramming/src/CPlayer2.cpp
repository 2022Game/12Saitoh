#include"CPlayer2.h"
#include "CApplication.h"

#define TEXCOORD 168, 188, 158, 130 //テクスチャマッピング
#define GRAVITY (TIPSIZE / 25.0f)   //重力加速度
#define JUMPV0 (TIPSIZE / 1.35f)     //ジャンプの初速
#define TEXCRY 196, 216, 158, 130   //テクスチャマッピング
#define TEXCOORD2 136,156,158,130   //右向き2
#define TEXLEFT1 188,168,158,130    //左向き1
#define TEXLEFT2 156,136,158,130    //左向き2
#define SOUND_JUMP "res\\jump.wav"  //ジャンプSE
//#define VELOCITY 4.0f             //移動速度
//#define HP 3                      //HPの初期値は3

CPlayer2::CPlayer2(float x, float y, float w, float h, CTexture* pt)
	:mInvincible(0)
	,interval(0)
{
	Set(x, y, w, h);
	Texture(pt, TEXCOORD);
	mTag = ETag::EPLAYER;
	mspState = Espstate::NULLSTATE;
	sHp = 1;
	sgoal = 1;
	mSoundJump.Load(SOUND_JUMP);
	spInstance = this;
}

void CPlayer2::Update()
{
	if (interval > 0)
	{
		//ジャンプのインターバル
		interval--;
		if (interval < 25)
		{
			mState = EState::EFOLL;
		}
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
		if (mState != EState::EFOLL)
		{
			if (interval == 0)
			{
				if (mspState == Espstate::ESPMOVE)
				{
					if (mInput.Key('W'))
					{
						mVy = JUMPV0;
						mSoundJump.Play();
						mState = EState::EJUMP;
						interval = 30;
					}
					else if (mInput.Key('J'))
					{
						mVy = JUMPV0;
						mSoundJump.Play();
						mState = EState::EJUMP;
						interval = 30;
					}
				}
			}
		}
		mspState = Espstate::NULLSTATE;
	}
	//Y軸速度に重力を減算する
	mVy -= GRAVITY;
	//Y座標にY軸速度を加える
	Y(Y() + mVy);
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
					if (o->State() != EState::ECRY)
					{
						mVy = JUMPV0;
					}
					else if (o->State() == EState::ECRY)
					{
						mVy = JUMPV0;
					}
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
			mspState = Espstate::ESPMOVE;
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
			if (interval > 0)
			{
				if (y > 0)
				{
					mState = EState::EFOLL;
				}
			}
		}
		break;
	case ETag::EWALL:
		if (CRectangle::Collision(o, &x, &y))
		{
			sHp--;
		}
		break;
	case ETag::EGOAL:
		if (CRectangle::Collision(o, &x, &y))
		{
			sgoal--;
		}
	}

}

int CPlayer2::sHp = 0;  //HP

int CPlayer2::Hp()
{
	return sHp;
}

int CPlayer2::sgoal = 0; //ゴール判定値
int CPlayer2::Goal()
{
	return sgoal;
}

CPlayer2* CPlayer2::spInstance = nullptr;
CPlayer2* CPlayer2::Instance()
{
	return spInstance;
}
