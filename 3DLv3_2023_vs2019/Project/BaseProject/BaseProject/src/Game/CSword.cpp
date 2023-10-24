#include "CSword.h"

// 剣モデル
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
	if (mpPlayer != nullptr)
	{
		delete mpPlayer;
		mpPlayer = nullptr;
	}
}

void CSword::Update()
{
	Position(mpPlayer->Position());
}

void CSword::Render()
{
	mpModel->Render(Matrix());
}
