#pragma once
#include "CObjectBase.h"
#include "CColliderMesh.h"

#define FIELD_RADIUS 625.0f	// 闘技場の半径

class CField : public CObjectBase
{
public:
	CField();
	~CField();

	// フィールドの床のコライダーを取得
	CColliderMesh* GetFieldCol() const;
	void Update();
	void Render();
	// フィールとレイの衝突判定
	bool CollisionRay(const CVector start, const CVector end, float* outDistance);

private:
	CModel* mpModel;

	CModel* mpColModel;
	CColliderMesh* mpColliderMesh;
};