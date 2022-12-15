#include "CFallBlock.h"
#include "CApplication.h"
#define FOLLBOCK_TEXCOORD 0, 48, 128, 80 //�e�N�X�`�����W
#define GRAVITY (TIPSIZE / 25.0f) //�d�͉����x


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

int CFallBlock::sfall = 0; //��������l
int CFallBlock::Fall()
{
	return sfall;
}