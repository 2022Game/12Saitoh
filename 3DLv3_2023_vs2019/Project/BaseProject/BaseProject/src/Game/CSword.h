#pragma once
#include "CModel.h"
#include "CObjectBase.h"
#include "CPlayer.h"

class CSword :public CObjectBase
{
public:
	CSword(const CVector& pos, const CVector& scale);
	~CSword();

	void Update();
	void Render();

private:
	CModel* mpModel;
	CPlayer* mpPlayer;
};