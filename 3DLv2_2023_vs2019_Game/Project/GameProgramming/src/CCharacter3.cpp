#include "CCharacter3.h"
#include "NavNode.h"
#include "NavManager.h"

#define GRAVITY CVector(0.0f,0.5f,0.0f)

CCharacter3::ETag CCharacter3::Tag()
{
	return mTag;
}

CCharacter3::CCharacter3()
	: mpModel(nullptr)
	, mTag(EZERO)
	, mGravity(0.0f, 0.0f, 0.0f)
	, mpNode(nullptr)
	, mHp(0)
{
	mGravity = GRAVITY;
	SetPauseType(TaskPauseType::eGame);
	//�^�X�N���X�g�ɒǉ�
	CTaskManager::Instance()->Add(this);
}

CCharacter3::CCharacter3(int priority)
	: mHp(0)
	, mGravity(CVector(0.0f, 0.0f, 0.0f))
	, mpModel(nullptr)
	, mpNode(nullptr)
	, mTag(ETag::EZERO)
{

	mPriority = priority;
	SetPauseType(TaskPauseType::eGame);
	CTaskManager::Instance()->Add(this);
}

CCharacter3::~CCharacter3()
{
	//�^�X�N���X�g����폜
	CTaskManager::Instance()->Remove(this);
}

void CCharacter3::Model(CModel* m)
{
	mpModel = m;
}

//�X�V����
void CCharacter3::Update()
{
	if (mpNode != nullptr)
	{
		mpNode->SetPos(Position());
	}
}

//�`�揈��
void CCharacter3::Render()
{
	mpModel->Render(mMatrix);
}

NavNode* CCharacter3::GetNavNode()const
{
	return mpNode;
}

//�L�����N�^�[������ł��邩�ǂ���
bool CCharacter3::IsDeath()
{
	return mHp <= 0;
}

int CCharacter3::HP()const
{
	return mHp;
}