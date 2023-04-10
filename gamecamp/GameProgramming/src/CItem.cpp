#include "CItem.h"
#include "CApplication.h"

#define TEXCOORD 4, 47, 49, 0

int CItem::mIh = 0;
int CItem::sNum = 0;

//�R���X�g���N�^
CItem::CItem(float x, float y, float w, float h, CTexture* pt)
	:CCharacter((int)ETaskPriority::ETITLE)
{
	Set(x, y, w, h);
	Texture(pt, TEXCOORD);
	mTag = ETag::EITEM;
	sNum++;
}

//�Փ˔���4
void CItem::Collision(CCharacter* m, CCharacter* o)
{
	////�߂荞�ݒ����ϐ���錾����
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

//�X�V����
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
