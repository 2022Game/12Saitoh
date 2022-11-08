#include "CTrap.h"
#include "CApplication.h"

#define TEXCOORD 222,255,204,169 //テクスチャマッピング
#define TEXCOORD2 0.0f, 48.0f, 128.0f, 80.0f //テクスチャ座標

CTrap::CTrap(float x, float y, float w, float h, CTexture* pt)
{
	Set(x, y, w*1.1, h/1.1);
	Texture(pt, TEXCOORD);
	mTag = ETag::ETRAP;
}

void CTrap::Collision()
{
	CApplication::CharacterManager()->Collision(this);
}

void CTrap::Collision(CCharacter* m, CCharacter* o)
{
	float x, y;
	switch (o->Tag())
	{
	case ETag::EPLAYER:
		if (CRectangle::Collision(o, &x, &y))
		{
			if (y > 0.0f)
			{
				if (CPlayer2::Instance()->State() != EState::EFOLL)
				{
					if (CPlayer2::Instance()->State() == EState::EJUMP)
					{
						Texture(Texture(), TEXCOORD2);
						mTag = ETag::EBLOCK;
					}
				}
			}
		}
		break;
	case ETag::EENEMY:
		break;
	case ETag::EBLOCK:
		break;
	}
}