#include "CSky.h"
#include "CModel.h"

CSky::CSky()
	: CObjectBase(ETag::eField, ETaskPriority::eBackground)
{
	mpModel = CResourceManager::Get<CModel>("Sky");
}

CSky::~CSky()
{
}

void CSky::Update()
{
#ifdef _DEBUG
	//CVector pos = Position();
	//CDebugPrint::Print("コロシアムの原点 \nX座標 : %.0f Y座標 : %.0f Z座標 : %.0f\n"
	//	, pos.X(), pos.Y(), pos.Z());
#endif
}

void CSky::Render()
{
	mpModel->Render(Matrix());
}