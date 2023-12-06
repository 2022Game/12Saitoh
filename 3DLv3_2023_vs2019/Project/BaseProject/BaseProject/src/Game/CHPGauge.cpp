#include "CHPGauge.h"
#include "CImage.h"
#include "Maths.h"
#include "CPlayer.h"

// �R���X�g���N�^
CHPGauge::CHPGauge()
{
	// HP�ݒ�
	// �t���[��
	mpHPFrameImage = new CImage(FRAME_IMAGE);
	mpHPFrameImage->SetSize(FRAME_SIZE_X, FRAME_SIZE_Y);
	// �΃Q�[�W�o�[
	mpHPBarImage = new CImage(GAUGE_BAR);
	mpHPBarImage->SetSize(BARSIZE_X, BARSIZE_Y);
	// �ԃQ�[�W�o�[
	mpRedBarImage = new CImage(GAUGE_BAR);
	mpRedBarImage->SetSize(BARSIZE_X, BARSIZE_Y);
}

// �f�X�g���N�^
CHPGauge::~CHPGauge()
{
}

void CHPGauge::Update_RedBar()
{
	// �ԃQ�[�W�̐ݒ�
	mpRedBarImage->SetPos(
		CVector2(mPosition.X() + mpHPBarImage->GetSize().X() + FRAME_BORDER,
			mPosition.Y() + FRAME_BORDER));
	int value = CPlayer::Instance()->GetTemporaryDamage();
	SetMaxValue(mMaxValue);
	// �b��_���[�W����ԃQ�[�W�̃o�[�T�C�Y��ύX
	CVector2 size = CVector2(BARSIZE_X, BARSIZE_Y);
	float percent = Math::Clamp01((float)value / mMaxValue);
	size.X(BARSIZE_X * percent);
	mpRedBarImage->SetSize(size);

	// �F�̐ݒ�
	CColor color;
	// �ԐF�ɐݒ�
	color = CColor(1.0f, 1.0f, 1.0f);
	mpRedBarImage->SetColor(color);
}

// �X�V
void CHPGauge::Update()
{
	// �Q�[�W�̃t���[���ƃo�[�̈ʒu��ݒ�
	mpHPFrameImage->SetPos(mPosition);
	mpHPBarImage->SetPos(mPosition + CVector2(FRAME_BORDER, FRAME_BORDER));

	// �΃Q�[�W�̃o�[�̃T�C�Y���ő�l�ƌ��ݒl���狁�߂�
	CVector2 size = CVector2(BARSIZE_X, BARSIZE_Y);
	float percent = Math::Clamp01((float)mValue / mMaxValue);
	size.X(BARSIZE_X * percent);
	mpHPBarImage->SetSize(size);

	// �ԃQ�[�W�̍X�V���s��
	Update_RedBar();

	// HP�o�[�̊����Ńo�[�̐F��ύX
	CColor color;
	// �ΐF�ɐݒ�
	color = CColor(0.0f, 1.0f, 0.0f);

	// �o�[�ɐF��ݒ�
	mpHPBarImage->SetColor(color);
}