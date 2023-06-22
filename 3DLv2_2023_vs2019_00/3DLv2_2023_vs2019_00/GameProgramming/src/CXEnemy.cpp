#include "CXEnemy.h"

//�R���X�g���N�^
CXEnemy::CXEnemy()
	: mColSpherHead(this, nullptr, CVector(0.0f, 5.0f, -3.0f), 0.5f)
	, mColSpherBody(this, nullptr, CVector(), 0.5f)
	, mColSpherSword(this, nullptr, CVector(-10.0f, 10.0f, 50.0f), 0.3f)
{

}

void CXEnemy::Init(CModelX* model)
{
	CXCharacter::Init(model);
	//�����s��̐ݒ�
	//��
	mColSpherHead.Matrix(&mpCombinedMatrix[11]);
	//��
	mColSpherBody.Matrix(&mpCombinedMatrix[8]);
	//��
	mColSpherSword.Matrix(&mpCombinedMatrix[21]);
}

void CXEnemy::Update()
{
	CXCharacter::Update();
}