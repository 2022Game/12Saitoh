#include "CSword.h"
#include "Maths.h"

#define SWORD_MODEL	"Character\\Sword\\sword.obj","Character\\Sword\\sword.mtl"

CSword::CSword(const CVector &pos,const CVector &scale)
	: CObjectBase(ETag::eGear, ETaskPriority::eGear)
{
	mpModel->Load(SWORD_MODEL);
	Position(pos);
	Scale(scale);
}

CSword::~CSword()
{
	if (mpModel != nullptr)
	{
		delete mpModel;
		mpModel = nullptr;
	}
}

void CSword::Update()
{
}

void CSword::Render()
{
	mpModel->Render(Matrix());
}
