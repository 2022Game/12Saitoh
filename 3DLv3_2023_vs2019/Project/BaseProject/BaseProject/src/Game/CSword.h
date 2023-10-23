#pragma once
#include "CModel.h"
#include "CColliderMesh.h"
#include "CObjectBase.h"

class CSword :public CObjectBase
{
public:
	CSword(const CVector &pos,const CVector &scale);
	~CSword();

	void Update();
	void Render();

private:
	CModel* mpModel;
	CColliderMesh* mpColliderMesh;
};