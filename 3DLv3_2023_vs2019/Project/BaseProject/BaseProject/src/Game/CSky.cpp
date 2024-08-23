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
	//CDebugPrint::Print("�R���V�A���̌��_ \nX���W : %.0f Y���W : %.0f Z���W : %.0f\n"
	//	, pos.X(), pos.Y(), pos.Z());
#endif
}

void CSky::Render()
{
	mpModel->Render(Matrix());
}