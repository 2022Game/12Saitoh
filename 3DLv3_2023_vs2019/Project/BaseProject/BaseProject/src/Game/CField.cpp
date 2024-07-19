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
#ifdef _DEBUG
	//CVector pos = Position();
	//CDebugPrint::Print("コロシアムの原点 \nX座標 : %.0f Y座標 : %.0f Z座標 : %.0f\n"
	//	, pos.X(), pos.Y(), pos.Z());
#endif
}

void CField::Render()
{
	mpModel->Render(Matrix());
}

// フィールドとレイの衝突処理
bool CField::CollisionRay(const CVector start, const CVector end, float* outDistance)
{
	CHitInfo hit;
	// フィールドのColliderMeshとの当たり判定を行う
	bool isHit = CCollider::CollisionRay
	(
		mpColliderMesh,
		start, end, &hit
	);

	// 衝突していたら衝突地点までの距離を設定
	if (isHit)
	{
		*outDistance = hit.dist;
	}

	return isHit;
}