#include "CWall.h"
#include "CApplication.h"

#define SnowBall "res\\Wall.obj","res\\Wall.mtl"


CWall::CWall()
	: mCollider(this, &mMatrix, CVector(), 0.3f)
{
	mModelWall.Load(SnowBall);
}

//���Ɖ��s���̐ݒ�
//Set(��,���s)
void CWall::Set(float w, float d)
{
	//�X�P�[���ݒ�
	mScale = CVector(0.3f, 0.3f, 0.3f);
}

//�X�V
void CWall::Update()
{
	CTransform::Update();
}

//�`��
void CWall::Render()
{
	mModelWall.Render(mMatrix);
}

void CWall::Collision()
{
	//�R���C�_�̗D��x�ύX
	mCollider.ChangePriority();
	//�Փˏ��������s
	CCollisionManager::Instance()->Collision(&mCollider, COLLISIONRANGE);
}

//�Փˏ���
//Collider(�R���C�_1,�R���C�_2)
void CWall::Collision(CCollider* m, CCollider* o)
{
}
