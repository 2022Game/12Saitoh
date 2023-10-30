#include "CSword.h"
#include "CCollisionManager.h"
#include "CPlayer.h"

// �����f��
#define SWORD_MODEL	"Character\\Sword\\sword.obj","Character\\Sword\\sword.mtl"

CSword::CSword()
{
	mpSword = new CModel();
	mpSword->Load(SWORD_MODEL);

}

CSword::~CSword()
{
	if (mpSword != nullptr)
	{
		delete mpSword;
		mpSword = nullptr;
	}
}

void CSword::CreateFieldObjects()
{
}

void CSword::Update()
{
	// �[����ԂƔ�����ԂŐe�̃{�[����ύX
	if (CPlayer::Instance()->IsDrawn())
	{
		SetAttachMtx(CPlayer::Instance()->GetFrameMtx("Armature_weapon_r"));
	}
	else
	{
		SetAttachMtx(CPlayer::Instance()->GetFrameMtx("Armature_sword_holder"));
	}
}

void CSword::Render()
{
	if (mpAttachMtx == nullptr)
	{
		mpSword->Render(Matrix());
	}
	else
	{
		CMatrix sm;
		CMatrix rm;
		sm.Scale(12.0f, 12.0f, 12.0f);
		// �[����ԂƔ�����Ԃ����肵�A�`��̒������s��
		if (CPlayer::Instance()->IsDrawn()) //����
		{
			//rm.RotateZ(180.0f);
			//rm.RotateY(180.0f);
			mpSword->Render(sm * (*mpAttachMtx));
		}
		else //�[��
		{
			rm.RotateX(-90.0f);
			mpSword->Render(rm * sm * (*mpAttachMtx));
		}
	}
}