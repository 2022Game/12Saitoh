#include "CSword.h"
#include "CCollisionManager.h"
#include "CPlayer.h"

CSword::CSword()
{
	mpSword = CResourceManager::Get<CModel>("Sword");

}

CSword::~CSword()
{
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
			rm = rm.RotateY(-90.0f);
			mpSword->Render( rm *  sm * (*mpAttachMtx));
		}
		else //�[��
		{
			rm.RotateX(-90.0f);
			mpSword->Render(rm * sm * (*mpAttachMtx));
		}
	}
}