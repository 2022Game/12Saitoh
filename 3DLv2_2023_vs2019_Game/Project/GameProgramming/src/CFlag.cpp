#include "CFlag.h"
#include "CCollisionManager.h"
#include "CEffect.h"
#include "CColliderMesh.h"

//�R���X�g���N�^
//CFlag(���f���A�ʒu�A��]�A�g�k�j
CFlag::CFlag(CModel* model, const CVector& position,
	const CVector& rotation, const CVector& scale)
	: mCollider(this, &mMatrix, CVector(0.0f, 10.0f, 0.0f), 1.0f, CCollider::EColliderTag::EFLAG)
{
	//���f���A�ʒu�A��]�A�g�k��ݒ肷��
	mpModel = model; //���f���̐ݒ�
	mPosition = position; //�ʒu�̐ݒ�
	mRotation = rotation; //��]�̐ݒ�
	mScale = scale; //�g�k�̐ݒ�
}

//�X�V����
void CFlag::Update()
{
	CTransform::Update();
}

//�Փˏ���
//CCollision(�R���C�_�P�A�R���C�_�Q�j
void CFlag::Collision(CCollider* m, CCollider* o)
{
	if (CCollider::Collision(m, o)) {
		if (o->ColliderTag() == CCollider::EColliderTag::EPLAYER)
		{
			mEnabled = false;
		}
	}
}

void CFlag::Collision()
{
	//�R���C�_�̗D��x�ύX
	mCollider.ChangePriority();
	//�Փˏ��������s
	CCollisionManager::Instance()->Collision(&mCollider, COLLISIONRANGE);}