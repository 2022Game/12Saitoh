#include "CCharacter3.h"
#include"CApplication.h"

#define GRAVITY CVector(0.0f,0.5f,0.0f)

CCharacter3::CCharacter3()
	: mpModel(nullptr)
	, mTag(EZERO)
	, mGravity(0.0f,0.0f,0.0f)
{
	//タスクリストに追加
	CTaskManager::Instance()->Add(this);
	mGravity = GRAVITY;
}
void CCharacter3::Model(CModel* m)
{
	mpModel = m;
}

//描画処理
void CCharacter3::Render()
{
	mpModel->Render(mMatrix);
}

CCharacter3::~CCharacter3()
{
	//タスクリストから削除
	CTaskManager::Instance()->Remove(this);
}

CCharacter3::CCharacter3(int priority)
	: mpModel(nullptr)
{
	mPriority = priority;
	CTaskManager::Instance()->Add(this);
}

CCharacter3::ETag CCharacter3::Tag()
{
	return mTag;
}