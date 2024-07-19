#include "CSPGauge.h"
#include "CImage.h"
#include "Maths.h"

// �R���X�g���N�^
CSPGauge::CSPGauge()
	: mZeroFlag(false)
{
	// SP�ݒ�
	mpSPFrameImage = new CImage(FRAME_IMAGE_PLAYER);
	mpSPFrameImage->SetSize(FRAME_SIZE_X, FRAME_SIZE_Y);
	mpSPBarImage = new CImage(GAUGE_BAR_PLAYER);
	mpSPBarImage->SetSize(BARSIZE_X, BARSIZE_Y);
}

// �f�X�g���N�^
CSPGauge::~CSPGauge()
{
	mpSPFrameImage->Kill();
	mpSPBarImage->Kill();
}

// �_�b�V�����̃X�^�~�i�񕜒��̃t���O���擾
void CSPGauge::SetSPZeroFlag(bool flag)
{
	mZeroFlag = flag;
}

// �X�V
void CSPGauge::Update()
{
	// �Q�[�W�̃t���[���ƃo�[�̈ʒu��ݒ�
	mpSPFrameImage->SetPos(mPosition);
	mpSPBarImage->SetPos(mPosition + CVector2(FRAME_BORDER, FRAME_BORDER));

	// �o�[�̃T�C�Y���ő�l�ƌ��ݒl���狁�߂�
	CVector2 size = CVector2(BARSIZE_X, BARSIZE_Y);
	float percent = Math::Clamp01(mValueF / mMaxValue);
	size.X(BARSIZE_X * percent);
	mpSPBarImage->SetSize(size);

	// SP�o�[�̊����Ńo�[�̐F��ύX
	CColor color;
	// �񕜎�
	if (mZeroFlag)
	{
		// �񕜒��͔����ԐF�ŕ`��
		color = CColor(0.9, 0.4f, 0.0f);
	}
	// �ʏ펞
	else
	{
		// 10%�ȉ��͐ԐF
		if (percent <= 0.3f) color = CColor(1.0f, 0.0f, 0.0f);
		// ����ȊO�͉��F
		else color = CColor(1.0f, 1.0f, 0.0f);
	}
	// �o�[�ɐF��ݒ�
	mpSPBarImage->SetColor(color);
}