#include "CSword.h"

#define SWORD_MODEL	"Character\\Sword\\sword.obj","Character\\Sword\\sword.mtl"

CSword::CSword(const CVector &pos,const CVector &scale)
	: CObjectBase(ETag::eGear, ETaskPriority::eGear)
{
	mpModel = new CModel();
	mpModel->Load(SWORD_MODEL);
	Position(pos);
	Scale(scale);

	// 親をプレイヤーに設定
	mpPlayer = CPlayer::Instance();
	SetParent(mpPlayer);
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
