#include "CFallBlock.h"
#include "CApplication.h"
#define FOLLBOCK_TEXCOORD 49.0f, 95.0f, 63.0f, 40.0f //�e�N�X�`�����W
#define GRAVITY (TIPSIZE / 30.0f) //�d�͉����x


CFallBlock::CFallBlock(float x, float y, float w, float h, CTexture* pt)
{
	Set(x, y, w*3, h*3);
	Texture(pt, FOLLBOCK_TEXCOORD);
	mTag = ETag::EFOLLBLOCK;
	sfall = 1;
}

void CFallBlock::Update()
{
	if ((sfall == 0))
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
	case ETag::EFOLLBLOCK:
		break;
	case ETag::EENEMY:
		break;
	}
}

int CFallBlock::Fall()
{
	return sfall;
}