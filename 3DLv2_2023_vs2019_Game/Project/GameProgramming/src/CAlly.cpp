#include "CAlly.h"
#include "CEffect.h"
#include "CApplication.h"
#include "CPlayer.h"
#include "CCollisionManager.h"

#define HP 3						//�ϋv�l
#define VELOCITY 0.1f				//���x
#define OBJ "res\\SnowGolem.obj"	//���f���̃t�@�C��
#define MTL "res\\SnowGolem.mtl"	//���f���̃}�e���A���t�@�C��
#define GRAVITY CVector(0.0f, 0.1f, 0.0f)	//�d��


CModel CAlly::sModel;				//���f���f�[�^�쐬

//�f�t�H���g�R���X�g���N�^
CAlly::CAlly()
	:CCharacter3(1)
	, mCollider1(this, &mMatrix, CVector(0.0f, 70.0f, 0.0f), 0.4f, CCollider::EColliderTag::EALL)
	, mCollider2(this, &mMatrix, CVector(0.0f, 45.0f, 0.0f), 0.36f, CCollider::EColliderTag::EALL)
	, mCollider3(this, &mMatrix, CVector(0.0f, 13.0f, 0.0f), 0.45f, CCollider::EColliderTag::EALL)
	, mHp(HP)
{
	//���f�����������͓ǂݍ���
	if (sModel.Triangles().size() == 0)
	{
		sModel.Load(OBJ, MTL);
	}
	//���f���f�[�^�̃|�C���^�ݒ�
	mpModel = &sModel;
}

//�R���X�g���N�^
//CAlly(�ʒu,��],�g�k)
CAlly::CAlly(const CVector& position, const CVector& rotation,
	const CVector& scale)
	:CAlly() //�f�t�H���g�R���X�g���N�^�����s����
{
	//�ʒu,��],�g�k��ݒ肷��
	mPosition = position; //�ʒu�̐ݒ�
	mRotation = rotation; //��]�̐ݒ�
	mScale = scale;       //�g�k�̐ݒ�
	CTransform::Update(); //�s��̍X�V
	//�ڕW�n�_�̐ݒ�
	mPoint = mPosition + CVector(0.0f, 0.0f, 100.0f) * mMatrixRotate;
}

//�X�V����
void CAlly::Update()
{
	mPosition = mPosition - GRAVITY;
	CTransform::Update();
}


void CAlly::Collision()
{
	//�R���C�_�̗D��x�ύX
	mCollider1.ChangePriority();
	mCollider2.ChangePriority();
	mCollider3.ChangePriority();
	//�Փˏ��������s
	CCollisionManager::Instance()->Collision(&mCollider1, COLLISIONRANGE);
	CCollisionManager::Instance()->Collision(&mCollider2, COLLISIONRANGE);
	CCollisionManager::Instance()->Collision(&mCollider3, COLLISIONRANGE);
}

//�Փˏ���
//Collision(�R���C�_1,�R���C�_2)
void CAlly::Collision(CCollider* m, CCollider* o)
{
	//����̃R���C�_�^�C�v�̔���
	switch (o->Type())
	{
	case CCollider::ESPHERE: //���R���C�_�̎�
		//�R���C�_��m��y���Փ˂��Ă��邩����
		if (CCollider::Collision(m, o))
		{
			if (o->ColliderTag() == CCollider::EColliderTag::EENEMYBULLET)
			{
				mHp--; //�q�b�g�|�C���g�̌��Z
				//�G�t�F�N�g����
				new CEffect(o->Parent()->Position(), 1.0f, 1.0f, "exp.tga", 4, 4, 2);
			}
		}
		break;
	case CCollider::ETRIANGLE: //�O�p�R���C�_�̎�
		CVector adjust; //�����l
		//�O�p�R���C�_�Ƌ��R���C�_�̏Փ˔���
		if (CCollider::CollisionTriangleSphere(o, m, &adjust))
		{
			//�Փ˂��Ă��Ȃ��ʒu�܂Ŗ߂�
			mPosition = mPosition + adjust;
			//���j�Œn�ʂɏՓ˂���Ɩ���
			if (mHp <= 0)
			{
				mEnabled = false;
			}
		}
		break;
	}
}