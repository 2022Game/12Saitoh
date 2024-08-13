#include "CTitleSword.h"
#include "CTitleModel.h"

// �R���X�g���N�^
CTitleSword::CTitleSword()
{
	mpModel = CResourceManager::Get<CModel>("Sword");
}

// �f�X�g���N�^
CTitleSword::~CTitleSword()
{
}

// �X�V����
void CTitleSword::Update()
{
	// �[����ԂƔ�����ԂŐe�̃{�[����ύX�^��
	if (CTitleModel::Instance()->IsDrawn())
	{
		SetAttachMtx(CTitleModel::Instance()->GetFrameMtx("Armature_weapon_r"));
	}
	else
	{
		SetAttachMtx(CTitleModel::Instance()->GetFrameMtx("Armature_sword_holder"));
	}
}

// �`�揈��
void CTitleSword::Render()
{
	mpModel->Render(Matrix());
}

// ����̍s��擾
CMatrix CTitleSword::Matrix() const
{
	// ��Ɏ����Ă��Ȃ����́A�������g�̍s���Ԃ�
	if (mpAttachMtx == nullptr)
	{
		return CTransform::Matrix();
	}
	// ��Ɏ����Ă��鎞�́A�A�^�b�`���Ă���s���Ԃ�
	else
	{
		CMatrix sm;
		CMatrix rm;
		sm.Scale(12.0f, 12.0f, 12.0f);
		// �[����ԂƔ�����Ԃ����肵�A�`��̒������s��
		if (CTitleModel::Instance()->IsDrawn()) //����
		{
			rm = rm.RotateY(-90.0f);
			return rm * sm * (*mpAttachMtx);
		}
		else //�[��
		{
			rm.RotateX(-90.0f);
			return rm * sm * (*mpAttachMtx);
		}
	}
}