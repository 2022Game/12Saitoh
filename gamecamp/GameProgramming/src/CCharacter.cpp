#include "CCharacter.h"
#include"CApplication.h"

CCharacter::CCharacter()
	: mpTexture(nullptr)
	, mLeft(0)
	, mRight(0)
	, mBottom(0)
	, mTop(0)
	, mTag(ETag::EZERO)
{
	//タスクリストに追加
	CTaskManager::Instance()->Add(this);
}

void CCharacter::Texture(CTexture* pTexture,
	int left, int right, int bottom, int top)
{
	mpTexture = pTexture;
	mLeft = left;
	mRight = right;
	mBottom = bottom;
	mTop = top;
}
//描画処理
void CCharacter::Render()
{
	mpTexture->DrawImage(
		X() - W(),
		X() + W(),
		Y() - H(),
		Y() + H(),
		mLeft, mRight, mBottom, mTop);
}

CCharacter::~CCharacter()
{
	//タスクリストから削除
	CTaskManager::Instance()->Remove(this);
}

CCharacter::CCharacter(int priority)
{
	mPriority = priority;
	CTaskManager::Instance()->Add(this);
}

CCharacter::ETag CCharacter::Tag()
{
	return mTag;
}

CTexture* CCharacter::Texture()
{
	return mpTexture;
}

bool CCharacter::Enabled()
{
	return mEnabled;
}