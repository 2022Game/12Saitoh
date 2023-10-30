#pragma once
#include "CObjectBase.h"
#include "CColliderMesh.h"
#include "CWeapon.h"

class CSword : public CWeapon
{
public:
	CSword();
	~CSword();

	void Update();
	void Render();

private:
	void CreateFieldObjects();

	CModel* mpSword;
};