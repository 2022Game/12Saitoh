#include "CBackGround.h"
#include "CApplication.h"

bool CBackGround::mMoveStage = false;

void CBackGround::Update()
{
	if (CGame::Num() == 0)
	{
		if (CApplication::StageCount() <= 430)
		{
			float x = X() - 2.0f;
			X(x);
			mMoveStage = true;
		}
		else {
			mMoveStage = false;
		}
	}
	else
	{
		mMoveStage = false;
	}
}

CBackGround::CBackGround(float x, float y, float w, float h, float l, float r, float b, float t, CTexture* pt)
	: CCharacter((int)ETaskPriority::EBackGround)
{
	Set(x, y, w, h);
	Texture(pt, l, r, b, t);
}

bool CBackGround::GetMoveStage()
{
	return mMoveStage;
}