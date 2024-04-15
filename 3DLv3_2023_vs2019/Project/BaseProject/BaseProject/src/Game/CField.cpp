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

// フィールドとレイの衝突処理
bool CField::CollisionRay(const CVector start, const CVector end, float* outDistance)
{
	CHitInfo hit;
	CMatrix m = Matrix();
	float length = 0.0f;
	std::vector<CTriangle> tris = mpColModel->Triangles();
	for (CTriangle tri : tris)
	{
		// 三角形のそれぞれの頂点を取得
		CVector tv[3] = { tri.V0() * m, tri.V1() * m, tri.V2() * m };
		// 線分(レイ)と三角形の衝突処理
		if (CCollider::CollisionTriangleLine(tv[1], tv[2], tv[3], start, end, &hit, false))
		{
			// 衝突した地点との距離を取得
			length = hit.dist;
		}
	}

	// レイが衝突していた場合
	if (length > 0.0f)
	{
		// 衝突位置までの距離を取得
		*outDistance = length;
		return true;
	}

	// 衝突していなかった場合、レイの長さをそのまま取得
	*outDistance = (end - start).Length();
	return false;
}