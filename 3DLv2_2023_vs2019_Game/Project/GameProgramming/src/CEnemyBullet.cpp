#include "CEnemyBullet.h"
#include "CApplication.h"
#include <cmath>
#include "CCollisionManager.h"


#define VELOCITYZ CVector(0.0f, 0.0f, 1.5f)	//Z���ړ�
#define GRAVITY CVector(0.0f,-0.1f,0.0f) //�d��
#define SnowBall "res\\SnowBall.obj","res\\SnowBall.mtl"

CEnemyBullet* CEnemyBullet::spinstence = nullptr;

//�R���X�g���N�^
CEnemyBullet::CEnemyBullet(CCollider::EColliderTag tag)
	: mCollider(this, &mMatrix, CVector(), 0.3f, tag)
	, mV(0.0f, 0.0f, 0.0f)

{
	mModelBall.Load(SnowBall);
	spinstence = this;
}

//���Ɖ��s���̐ݒ�
//Set(��,���s)
void CEnemyBullet::Set(float w, float d)
{
	//�X�P�[���ݒ�
	mScale = CVector(0.3f, 0.3f, 0.3f);
	//�O�p�`�̒��_�ݒ�
	//mT.Vertex(CVector(w, 0.0f, 0.0f), CVector(0.0f, 0.0f, -d), CVector(-w, 0.0f, 0.0f));
	//�O�p�`�̖@���ݒ�
	//mT.Normal(CVector(0.0f, 1.0f, 0.0f));
}

//�X�V
void CEnemyBullet::Update()
{
	CTransform::Update();
	//�ʒu�X�V
	mPosition = mPosition + (VELOCITYZ * mMatrixRotate) + (mV * mMatrixRotate);
	mV = mV + CVector(0.0f, -0.01f, 0.0f);
}

//�`��
void CEnemyBullet::Render()
{
	mModelBall.Render(mMatrix);
}

void CEnemyBullet::Collision()
{
	//�R���C�_�̗D��x�ύX
	mCollider.ChangePriority();
	//�Փˏ��������s
	CCollisionManager::Instance()->Collision(&mCollider, COLLISIONRANGE);
}

//�Փˏ���
//Collider(�R���C�_1,�R���C�_2)
void CEnemyBullet::Collision(CCollider* m, CCollider* o)
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
		if (o->ColliderTag() == CCollider::EColliderTag::EPLAYER)
		{
			if (CCollider::Collision(m, o))
			{
				mEnabled = false;
			}
		}
		break;
	}
}

CEnemyBullet* CEnemyBullet::Instance()
{
	return spinstence;
}