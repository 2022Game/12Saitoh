#include"CWall.h"
#include"CApplication.h"
#define TEXCOORD 222,255,204,169 //テクスチャマッピング

CWall::CWall(float x, float y, float w, float h, CTexture* pt)
{
	Set(x, y, w/2, h/2 );
	Texture(pt, TEXCOORD);
	mTag = ETag::EWALL;
}

void CWall::Collision()
{
	CApplication::CharacterManager()->Collision(this);
}

void CWall::Collision(CCharacter* m, CCharacter* o)
{
	switch (o->Tag())
	{
	case ETag::EENEMY:
		break;
	case ETag::EPLAYER:
		break;
	}
}