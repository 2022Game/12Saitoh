#include "CSlimeAttackBox.h"
#include "CApplication.h"

#define MU 0, 200, 0, 0　//仮
#define WOLFNTL 0,0,0,0 //仮


//デストラクタ
CSlimeAttackBox::~CSlimeAttackBox()
{
}
//コンストラクタ
CSlimeAttackBox::CSlimeAttackBox(float x, float y, float w, float h, CTexture* pt)
	:CCharacter((int)ETaskPriority::ECharacter)
{
	Set(x, y, w, h);
	Texture(pt, WOLFNTL);
	mTag = ETag::EATTACK;
}

//更新処理
void CSlimeAttackBox::Update()
{
	if (CApplication::Di() == 1)
	{
		mEnabled = false;
	}
	/*if()*/
	CCharacter::Update();
}