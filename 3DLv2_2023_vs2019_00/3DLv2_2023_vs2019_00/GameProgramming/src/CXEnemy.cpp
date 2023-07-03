#include "CXEnemy.h"

//�R���X�g���N�^
CXEnemy::CXEnemy()
	: mColSpherHead(this, nullptr, CVector(0.0f, 1.0f, 0.0f), 1.5f)
	, mColSpherBody(this, nullptr, CVector(0.5f, -1.0f, 0.0f), 1.0f)
	, mColSpherSword0(this, nullptr, CVector(0.7f, 3.5f, -0.2f), 0.5f)
	, mColSpherSword1(this, nullptr, CVector(0.5f, 2.5f, -0.2f), 0.5f)
	, mColSpherSword2(this, nullptr, CVector(0.3f, 1.5f, -0.2f), 0.5f)
{

}

void CXEnemy::Init(CModelX* model)
{
	CXCharacter::Init(model);
	//�����s��̐ݒ�
	//��
	mColSpherHead.Matrix(&mpCombinedMatrix[1]);
	//��
	mColSpherBody.Matrix(&mpCombinedMatrix[1]);
	//��
	mColSpherSword0.Matrix(&mpCombinedMatrix[26]);
	mColSpherSword1.Matrix(&mpCombinedMatrix[26]);
	mColSpherSword2.Matrix(&mpCombinedMatrix[26]);

}

void CXEnemy::Update()
{
	CXCharacter::Update();
}

void CXEnemy::Collision(CCollider* m, CCollider* o)
{
	//�����̃R���C�_�^�C�v
	switch (m->Type())
	{
	case CCollider::ESPHERE:	//���R���C�_
		/*
		����
		����̃R���C�_�[�����R���C�_
		�R���C�_�[�̐e���v���C���[
		�R���C�_�[������
		�����̃R���C�_����
		*/
		if (o->Type() == CCollider::ESPHERE &&
			o->Parent()->Tag() == ETag::EPLAYER &&
			o->Tag() == CCollider::ETag::ESWORD &&
			m->Tag() == CCollider::ETag::EBODY)
		{
			//�Փˏ���
			if (CCollider::Collision(m, o))
			{
				//�|�ꍞ�ރA�j���[�V����
				CXCharacter::ChangeAnimation(11, false, 60);
			}
		}
	}
}