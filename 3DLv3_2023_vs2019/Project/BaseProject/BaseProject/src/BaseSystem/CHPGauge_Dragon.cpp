#include "CHPGauge_Dragon.h"
#include "CImage.h"
#include "Maths.h"

// �R���X�g���N�^
CHPGauge_Dragon::CHPGauge_Dragon()
{
	// HP�ݒ�
	mpHPFrameImage = new CImage(FRAME_IMAGE_DRAGON);
	mpHPFrameImage->SetSize(DRAGON_FRAME_SIZE_X, DRAGON_FRAME_SIZE_Y);
	// �΃Q�[�W�o�[
	mpHPBarImage = new CImage(GAUGE_BAR_DRAGON);
	mpHPBarImage->SetSize(DRAGON_BARSIZE_X, DRAGON_BARSIZE_Y);
}

// �f�X�g���N�^
CHPGauge_Dragon::~CHPGauge_Dragon()
{
	mpHPFrameImage->Kill();
	mpHPBarImage->Kill();
}

// �X�V
void CHPGauge_Dragon::Update()
{
	// �Q�[�W�̃t���[���ƃo�[�̈ʒu��ݒ�
	mpHPFrameImage->SetPos(mPosition);
	mpHPBarImage->SetPos(mPosition + CVector2(DRAGON_FRAME_BORDER, DRAGON_FRAME_BORDER));

	// HP�Q�[�W�̃o�[�̃T�C�Y���ő�l�ƌ��ݒl���狁�߂�
	CVector2 size = CVector2(DRAGON_BARSIZE_X, DRAGON_BARSIZE_Y);
	float percent = Math::Clamp01((float)mValue / mMaxValue);
	size.X(DRAGON_BARSIZE_X * percent);
	mpHPBarImage->SetSize(size);
}