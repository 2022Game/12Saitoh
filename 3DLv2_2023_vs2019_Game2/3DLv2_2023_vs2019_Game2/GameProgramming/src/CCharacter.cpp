#include "CCharacter.h"
#include "CApplication.h"

CCharacter::CCharacter()
	:mpModel(nullptr)
	, mTag(EZERO)
{
	//�^�X�N���X�g�ɒǉ�
//	CApplication::TaskManager()->Add(this);
	CTaskManager::Instance()->Add(this);
}

CCharacter::CCharacter(int priority)
	: mpModel(nullptr)
	, mTag(EZERO)
{
	mPriority = priority;
	CTaskManager::Instance()->Add(this);
}

CCharacter::ETag CCharacter::Tag()
{
	return mTag;
}

CCharacter::~CCharacter()
{
	//�^�X�N���X�g����폜
//	CApplication::TaskManager()->Remove(this);
	CTaskManager::Instance()->Remove(this);
}

void CCharacter::Model(CModel* m)
{
	mpModel = m;
}

//�`�揈��
void CCharacter::Render()
{
	mpModel->Render(mMatrix);
}
