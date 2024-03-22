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

// フィールドの床のコライダーを取得
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