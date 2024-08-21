#include "CNormalSwordEffect.h"

// �R���X�g���N�^
CNormalSwordEffect::CNormalSwordEffect(const CVector& start, const CVector& end)
	: CLineEffect(ETag::eEffect)
{

	CVector2 size = CVector2(64.0f, 256.0f);
	// �e�N�X�`����ݒ�
	SetTexture("NormalSwordEffect");
	// �x�[�XUV��ݒ�
	SetBaseUV(CRect(0.0f, 0.0f, size.X(), size.Y()));
	// �G�t�F�N�g�̃A�j���[�V�����f�[�^�𐶐����Đݒ�
	SetAnimData(new TexAnimData(1, 5, false, 5, 0.05f));

	// �����G�t�F�N�g�̎n�_�ƏI�_��ݒ�
	float length = (start - end).Length();
	float width = length * (size.X() / size.Y());
	AddPoint(start, width, width);
	AddPoint(end, width, width);
}

// �f�X�g���N�^
CNormalSwordEffect::~CNormalSwordEffect()
{
	SAFE_DELETE(mpAnimData);
}

// �X�V����
void CNormalSwordEffect::Update()
{
	CLineEffect::Update();

	// �G�t�F�N�g�̃A�j���[�V�������I�������A�G�t�F�N�g���폜
	if (IsEndAnim())
	{
		Kill();
	}
}

// �`�揈��
void CNormalSwordEffect::Render()
{
	//glDisable(GL_DEPTH_TEST);
	CLineEffect::Render();
	//glEnable(GL_DEPTH_TEST);
}