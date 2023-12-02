#include "CHPGauge.h"
#include "CImage.h"
#include "Maths.h"

// �R���X�g���N�^
CHPGauge::CHPGauge()
{
	// HP�ݒ�
	mpHPFrameImage = new CImage(FRAME_IMAGE);
	mpHPFrameImage->SetSize(FRAME_SIZE_X, FRAME_SIZE_Y);
	mpHPBarImage = new CImage(GAUGE_BAR);
	mpHPBarImage->SetSize(BARSIZE_X, BARSIZE_Y);
}

// �f�X�g���N�^
CHPGauge::~CHPGauge()
{
}

// �X�V
void CHPGauge::Update()
{
	// �Q�[�W�̃t���[���ƃo�[�̈ʒu��ݒ�
	mpHPFrameImage->SetPos(mPosition);
	mpHPBarImage->SetPos(mPosition + CVector2(FRAME_BORDER, FRAME_BORDER));

	// �o�[�̃T�C�Y���ő�l�ƌ��ݒl���狁�߂�
	CVector2 size = CVector2(BARSIZE_X, BARSIZE_Y);
	float percent = Math::Clamp01((float)mValue / mMaxValue);
	size.X(BARSIZE_X * percent);
	mpHPBarImage->SetSize(size);

	// HP�o�[�̊����Ńo�[�̐F��ύX
	CColor color;
	// 10%�ȉ��͐ԐF
	if (percent <= 0.1f) color = CColor(1.0f, 0.0f, 0.0f);
	// 30%�ȉ��͉��F
	else if (percent <= 0.3f) color = CColor(1.0f, 1.0f, 0.0f);
	// ����ȊO�͗�
	else color = CColor(0.0f, 1.0f, 0.0f);

	// �o�[�ɐF��ݒ�
	mpHPBarImage->SetColor(color);
}