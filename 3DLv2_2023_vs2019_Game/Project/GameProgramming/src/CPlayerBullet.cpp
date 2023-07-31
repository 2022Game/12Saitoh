#include "CPlayerBullet.h"
#include "CApplication.h"
#include <cmath>
#include "CCollisionManager.h"


#define VELOCITYZ CVector(0.0f, 0.0f, 1.5f)	//Z���ړ�
#define GRAVITY CVector(0.0f,-0.1f,0.0f) //�d��
#define SnowBall "res\\SnowBall.obj","res\\SnowBall.mtl"

CPlayerBullet* CPlayerBullet::spinstence = nullptr;

//�R���X�g���N�^
CPlayerBullet::CPlayerBullet(CCollider::EColliderTag tag)
	: mCollider(this, &mMatrix, CVector(), 0.3f,tag)
	, mV(0.0f,0.0f,0.0f)

{
	mModelBall.Load(SnowBall);
	spinstence = this;
}

//���Ɖ��s���̐ݒ�
//Set(��,���s)
void CPlayerBullet::Set(float w, float d)
{
	//�X�P�[���ݒ�
	mScale = CVector(0.3f, 0.3f, 0.3f);
	//�O�p�`�̒��_�ݒ�
	//mT.Vertex(CVector(w, 0.0f, 0.0f), CVector(0.0f, 0.0f, -d), CVector(-w, 0.0f, 0.0f));
	//�O�p�`�̖@���ݒ�
	//mT.Normal(CVector(0.0f, 1.0f, 0.0f));
}

//�X�V
void CPlayerBullet::Update()
{
	CTransform::Update();
	//�ʒu�X�V
	mPosition = mPosition + (VELOCITYZ * mMatrixRotate) + (mV * mMatrixRotate);
	mV = mV + CVector(0.0f, -0.01f, 0.0f);
}

//�`��
void CPlayerBullet::Render()
{
	mModelBall.Render(mMatrix);
}

void CPlayerBullet::Collision()
{
	//�R���C�_�̗D��x�ύX
	mCollider.ChangePriority();
	//�Փˏ��������s
	CCollisionManager::Instance()->Collision(&mCollider, COLLISIONRANGE);
}

//�Փˏ���
//Collider(�R���C�_1,�R���C�_2)
void CPlayerBullet::Collision(CCollider* m, CCollider* o)
{
	//����̃R���C�_�^�C�v�̔���
	switch (o->Type())
	{
	case CCollider::ETRIANGLE: //�O�p�R���C�_�̎�
	{
		CVector adjust; //�����l
		//�O�p�R���C�_�Ƌ��R���C�_�̏Փ˔���
		if (CCollider::CollisionTriangleSphere(o, m, &adjust))
		{
			mEnabled = false;
		}
	}
	break;
	case CCollider::ESPHERE: //���R���C�_�̎�
		//���肪�G�̎�
		if (o->ColliderTag() == CCollider::EColliderTag::EENEMY)
		{
			if (CCollider::Collision(m, o))
			{
				mEnabled = false;
			}
		}
		break;
	}
}

CPlayerBullet* CPlayerBullet::Instance()
{
	return spinstence;
}