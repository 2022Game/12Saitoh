#include"CEnemy2.h"
#include "CApplication.h"

#define TEXCOORD 168, 188, 190, 160 //テクスチャマッピング
#define TEXCRY 196, 216, 190, 160   //テクスチャマッピング
#define GRAVITY (TIPSIZE / 20.0f)   //重力加速度
#define TEXCOORD2 136,156,190,160   //右向き2
#define TEXLEFT1 188,168,190,160   //左向き1
#define TEXLEFT2 156,136,190,160  //左向き2

CEnemy2::CEnemy2(float x, float y, float w, float h, CTexture* pt)
	:cooltime(0)
{
	Set(x, y, w, h);
	Texture(pt, TEXCOORD);
	mTag = ETag::EENEMY;
	//X軸速度の初期値を移動速度にする
	mVx = VELOCITY - 1;
	spInstance = this;
	cooltime = 30;
}

void CEnemy2::Update()
{
	const int PITCH = 32;  //画像を切り替える間隔
	if ((int)X() % PITCH < PITCH / 2)
		if (mVx > 0.0f)  //左へ移動
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
		if (mVx > 0.0f) //左へ移動
		{
			//左向き2を設定
			Texture(Texture(), TEXLEFT2);
		}
		else
		{
			//2番目の画像を設定
			Texture(Texture(), TEXCOORD2);
		}
	//Y座標にY軸速度を加える
	Y(Y() + mVy);
	//Y軸速度に重力を減算する
	mVy -= GRAVITY;
	if (mState == EState::ECRY)
	{
		cooltime--;
	}
	switch (mState)
	{
	case EState::ECRY:
		//泣く画像を設定
		Texture(Texture(), TEXCRY);
		//クールタイムが0になったら消滅
		if(cooltime == 0)
		mEnabled = false;
		break;
	case EState::EMOVE:
		if (CEnemy2::X() - CPlayer2::Instance()->X() < 1500)
		{
			//X軸速度分、X座標を更新する
			float x = X() - mVx;
			X(x);
		}
		break;
	}
}

void CEnemy2::Collision()
{
	CApplication::CharacterManager()->Collision(this);
}


void CEnemy2::Collision(CCharacter* m, CCharacter* o)
{
	//めり込み調整変数を宣言する
	float x, y;
	switch (o->Tag())
	{
	case ETag::ETURN:
		//折り返し当たった時
		if (CRectangle::Collision(o, &x, &y))
		{
			//めり込まない位置まで戻す
			X(X() + x);
			Y(Y() + y);
			//X軸速度を反転させる
			mVx = -mVx;
		}
		break;
	case ETag::EPLAYER:
		if (CRectangle::Collision(o))
		{
			if ((o->State() == EState::EJUMP)||
				(o->State() == EState::EFOLL))
			{
				mState = EState::ECRY;
			}
			else if (o->State() == EState::ECRY)
			{
				mState = EState::EMOVE;
			}
		}
		break;
	case ETag::EBLOCK:
		if (CRectangle::Collision(o, &x, &y))
		{
			X(X() + x);
			Y(Y() + y);
			if (mVy < -mVy)
			{
				mVy = -10.0;
			}
			//着地した時
			if (y != 0.0f)
			{
				//Y軸速度を0にする
				mVy = 0.0f;
				if (y > 0.0f)
				{
					if (mState != EState::ECRY)
					{
						mState = EState::EMOVE;
					}
				}
			}
		}
		break;
	case ETag::EFOLLBLOCK:
		if (CRectangle::Collision(o, &x, &y))
		{
			X(X() + x);
			Y(Y() + y);
			if (mVy < -mVy)
			{
				mVy = 0.0f;
			}
		}
	}
}

CEnemy2* CEnemy2::spInstance = nullptr;
CEnemy2* CEnemy2::Instance()
{
	return spInstance;
}