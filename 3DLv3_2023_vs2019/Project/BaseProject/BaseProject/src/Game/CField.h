#pragma once
#include "CObjectBase.h"
#include "CColliderMesh.h"

class CField : public CObjectBase
{
public:
	CField();
	~CField();

	// フィールドの床のコライダーを取得
	CColliderMesh* GetFieldCol() const;
	void Update();
	void Render();

private:
	CModel* mpModel;

	CModel* mpColModel;
	CColliderMesh* mpColliderMesh;

};