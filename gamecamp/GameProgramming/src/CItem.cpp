#include "CItem.h"
#include "CApplication.h"

#define TEXCOORD 4, 47, 49, 0

int CItem::mIh = 0;
int CItem::sNum = 0;

//コンストラクタ
CItem::CItem(float x, float y, float w, float h, CTexture* pt)
	:CCharacter((int)ETaskPriority::ETITLE)
{
	Set(x, y, w, h);
	Texture(pt, TEXCOORD);
	mTag = ETag::EITEM;
	sNum++;
}

//衝突判定4
void CItem::Collision(CCharacter* m, CCharacter* o)
{
	////めり込み調整変数を宣言する
	switch (o->Tag())
	{
	case ETag::EPLAYER:
		if (CRectangle::Collision(o))
		{
			mIh = 1;
		}
		break;
	}
}

//更新処理
void CItem::Update()
{
	if (mIh == 1)
	{
		float y = Y() + 1000000000000000000.0f;
		Y(y);
		mIh = 0;
		sNum--;
	}
	CCharacter::Update();
}

int CItem::Ih()
{
	return mIh;
}

int CItem::Num()
{
	return sNum;
}
