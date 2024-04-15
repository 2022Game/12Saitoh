#include "CField.h"
#include "CCollisionManager.h"
#include "CMoveFloor.h"
#include "CRotateFloor.h"

CField::CField()
	: CObjectBase(ETag::eField, ETaskPriority::eBackground)
{
	mpModel = CResourceManager::Get<CModel>("Field");

	mpColModel = CResourceManager::Get<CModel>("Cylinder");
	mpColliderMesh = new CColliderMesh(this, ELayer::eField, mpColModel,0, true);
}

CField::~CField()
{
	if (mpColliderMesh != nullptr)
	{
		delete mpColliderMesh;
		mpColliderMesh = nullptr;
	}
}

// �t�B�[���h�̏��̃R���C�_�[���擾
CColliderMesh* CField::GetFieldCol() const
{
	return mpColliderMesh;
}

void CField::Update()
{
}

void CField::Render()
{
	mpModel->Render(Matrix());
}

// �t�B�[���h�ƃ��C�̏Փˏ���
bool CField::CollisionRay(const CVector start, const CVector end, float* outDistance)
{
	CHitInfo hit;
	CMatrix m = Matrix();
	float length = 0.0f;
	std::vector<CTriangle> tris = mpColModel->Triangles();
	for (CTriangle tri : tris)
	{
		// �O�p�`�̂��ꂼ��̒��_���擾
		CVector tv[3] = { tri.V0() * m, tri.V1() * m, tri.V2() * m };
		// ����(���C)�ƎO�p�`�̏Փˏ���
		if (CCollider::CollisionTriangleLine(tv[1], tv[2], tv[3], start, end, &hit, false))
		{
			// �Փ˂����n�_�Ƃ̋������擾
			length = hit.dist;
		}
	}

	// ���C���Փ˂��Ă����ꍇ
	if (length > 0.0f)
	{
		// �Փˈʒu�܂ł̋������擾
		*outDistance = length;
		return true;
	}

	// �Փ˂��Ă��Ȃ������ꍇ�A���C�̒��������̂܂܎擾
	*outDistance = (end - start).Length();
	return false;
}