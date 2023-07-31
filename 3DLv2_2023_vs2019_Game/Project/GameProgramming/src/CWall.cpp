#include "CWall.h"
#include "CCollisionManager.h"
#include "CEffect.h"
#include "CColliderMesh.h"

//�R���X�g���N�^
//CWall(���f���A�ʒu�A��]�A�g�k�j
CWall::CWall(CModel* model, const CVector& position,
	const CVector& rotation, const CVector& scale)
{
	//���f���A�ʒu�A��]�A�g�k��ݒ肷��
	mpModel = model;		//���f���̐ݒ�
	mPosition = position;	//�ʒu�̐ݒ�
	mRotation = rotation;	//��]�̐ݒ�
	mScale = scale;			//�g�k�̐ݒ�
	mColliderMesh.Set(this, &mMatrix, mpModel);
}

//�X�V����
void CWall::Update()
{
	CTransform::Update();
}

//�Փˏ���
//CCollision(�R���C�_�P�A�R���C�_�Q�j
void CWall::Collision(CCollider* m, CCollider* o) {
	//if (CCollider::Collision(m, o)) {
	//	mEnabled = false;
	//}
}

void CWall::Collision()
{
	////�R���C�_�̗D��x�ύX
	//mCollider1.ChangePriority();
	//mCollider2.ChangePriority();
	//mCollider3.ChangePriority();
	////�Փˏ��������s
	//CCollisionManager::Instance()->Collision(&mCollider1, COLLISIONRANGE);
	//CCollisionManager::Instance()->Collision(&mCollider2, COLLISIONRANGE);
	//CCollisionManager::Instance()->Collision(&mCollider3, COLLISIONRANGE);
}

bool CWall::CollisionRay(const CVector& start, const CVector& end, float* outDistance)
{
	CVector lv[2] = { start, end };
	CVector adjust;
	CVector cross;
	float nearLength = -1.0f;
	CMatrix m = Matrix();
	std::vector<CTriangle> tris = mpModel->Triangles();
	for (CTriangle tri : tris)
	{
		CVector tv[3] = { tri.V0() * m, tri.V1() * m, tri.V2() * m };
		if (CCollider::CollisionTriangleLine(tv, lv, &adjust, &cross))
		{
			float length = (cross - start).Length();
			if (nearLength < 0.0f || length < nearLength)
			{
				nearLength = length;
			}
		}
	}
	if (nearLength >= 0.0f)
	{
		*outDistance = nearLength;
		return true;
	}
	return false;
}
