#include "CBullet.h"
#include "CApplication.h"
#include <cmath>

#define VELOCITYZ CVector(0.0f, 0.0f, 1.5f)	//Z���ړ�
#define GRAVITY CVector(0.0f,-0.1f,0.0f) //�d��
#define SnowBall "res\\SnowBall.obj","res\\SnowBall.mtl"


CBullet::CBullet()
	: mCollider(this, &mMatrix, CVector(), 0.3f,(int)EColliderTag::EBULLET)
	, mV(0.0f,0.0f,0.0f)
{
	mModelBall.Load(SnowBall);
}

//���Ɖ��s���̐ݒ�
//Set(��,���s)
void CBullet::Set(float w, float d)
{
	//�X�P�[���ݒ�
	mScale = CVector(0.3f, 0.3f, 0.3f);
	//�O�p�`�̒��_�ݒ�
	//mT.Vertex(CVector(w, 0.0f, 0.0f), CVector(0.0f, 0.0f, -d), CVector(-w, 0.0f, 0.0f));
	//�O�p�`�̖@���ݒ�
	//mT.Normal(CVector(0.0f, 1.0f, 0.0f));
}

//�X�V
void CBullet::Update()
{
	CTransform::Update();
	//�ʒu�X�V
	mPosition = mPosition + (VELOCITYZ * mMatrixRotate) + (mV * mMatrixRotate);
	mV = mV + CVector(0.0f, -0.01f, 0.0f);
}

//�`��
void CBullet::Render()
{
	mModelBall.Render(mMatrix);
}

void CBullet::Collision()
{
	//�R���C�_�̗D��x�ύX
	mCollider.ChangePriority();
	//�Փˏ��������s
	CCollisionManager::Instance()->Collision(&mCollider, COLLISIONRANGE);
}

//�Փˏ���
//Collider(�R���C�_1,�R���C�_2)
void CBullet::Collision(CCollider* m, CCollider* o)
{
	//����̃R���C�_�^�C�v�̔���
	switch (o->Type())
	{
	case CCollider::ETRIANGLE: //�O�p�R���C�_�̎�
		CVector adjust; //�����l
		//�O�p�R���C�_�Ƌ��R���C�_�̏Փ˔���
		if (CCollider::CollisionTriangleSphere(o, m, &adjust))
		{
			mEnabled = false;
		}
		break;
	}
}
