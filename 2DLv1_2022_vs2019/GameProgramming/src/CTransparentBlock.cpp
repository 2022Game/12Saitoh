#include "CTransparentBlock.h"
#include "CApplication.h"

#define TEXCOORD 115.0f,126.0f,46.0f,35.0f //テクスチャマッピング
#define TEXCOORD2 97.0f, 111.0f, 47.0f, 32.0f //テクスチャ座標

CTransparentBlock::CTransparentBlock(float x, float y, float w, float h, CTexture* pt)
{
	Set(x, y, w * 1.1, h / 1.1);
	Texture(pt, TEXCOORD);
	mTag = ETag::ETRAP;
}

void CTransparentBlock::Collision()
{
	CApplication::CharacterManager()->Collision(this);
}

void CTransparentBlock::Collision(CCharacter* m, CCharacter* o)
{
	float x, y;
	switch (o->Tag())
	{
	case ETag::EPLAYER:
		if (CRectangle::Collision(o, &x, &y))
		{
			if (y < 0.0f)
			{
				Texture(Texture(), TEXCOORD2);
				mTag = ETag::EBLOCK;
			}
		}
		break;
	case ETag::EENEMY:
		break;
	case ETag::EBLOCK:
		break;
	}
}