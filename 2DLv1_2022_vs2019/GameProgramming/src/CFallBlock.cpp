#include "CFallBlock.h"
#include "CApplication.h"
#define FOLLBOCK_TEXCOORD 0, 48, 128, 80 //テクスチャ座標
#define GRAVITY (TIPSIZE / 25.0f) //重力加速度


CFallBlock::CFallBlock(float x, float y, float w, float h, CTexture* pt)
{
	Set(x, y, w*5, h*3);
	Texture(pt, FOLLBOCK_TEXCOORD);
	mTag = ETag::EBLOCK;
	sfall = 1;
}

void CFallBlock::Update()
{
	if ((sfall == 0)||(mTag == ETag::ETRAP))
	{
		mVy -= GRAVITY;
		Y(Y() + mVy);
	}
}

void CFallBlock::Collision()
{
	CApplication::CharacterManager()->Collision(this);
}

void CFallBlock::Collision(CCharacter* m, CCharacter* o)
{
	float x, y;
	switch (o->Tag())
	{
	case ETag::EPLAYER:
		if (CRectangle::Collision(o, &x, &y))
		{
			sfall = 0;
		}
		break;
	case ETag::EBLOCK:
			break;
	}
}

int CFallBlock::sfall = 0; //落下判定値
int CFallBlock::Fall()
{
	return sfall;
}