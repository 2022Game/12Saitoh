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