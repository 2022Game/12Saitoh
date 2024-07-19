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
#ifdef _DEBUG
	//CVector pos = Position();
	//CDebugPrint::Print("�R���V�A���̌��_ \nX���W : %.0f Y���W : %.0f Z���W : %.0f\n"
	//	, pos.X(), pos.Y(), pos.Z());
#endif
}

void CField::Render()
{
	mpModel->Render(Matrix());
}

// �t�B�[���h�ƃ��C�̏Փˏ���
bool CField::CollisionRay(const CVector start, const CVector end, float* outDistance)
{
	CHitInfo hit;
	// �t�B�[���h��ColliderMesh�Ƃ̓����蔻����s��
	bool isHit = CCollider::CollisionRay
	(
		mpColliderMesh,
		start, end, &hit
	);

	// �Փ˂��Ă�����Փ˒n�_�܂ł̋�����ݒ�
	if (isHit)
	{
		*outDistance = hit.dist;
	}

	return isHit;
}