#include "CToukiGauge.h"
#include "CImage.h"
#include "Maths.h"

// �R���X�g���N�^
CToukiGauge::CToukiGauge()
{	
	// �t���[��
	mpFrameImage = new CImage
	(
		FRAME_IMAGE_PLAYER,
		ETaskPriority::eUI, 0, ETaskPauseType::eGame,
		false, false
	);
	mpFrameImage->SetSize(TOUKI_FRAME_SIZE_X, TOUKI_FRAME_SIZE_Y);
	// �Q�[�W�o�[
	mpBarImage = new CImage
	(
		GAUGE_BAR_PLAYER,
		ETaskPriority::eUI, 0, ETaskPauseType::eGame,
		false, false
	);
	mpBarImage->SetSize(TOUKI_BARSIZE_X, TOUKI_BARSIZE_Y);
	// �t���[���̋�؂�
	mpFrameSeparator = new CImage
	(
		FRAME_IMAGE_SEPARATOR,
		ETaskPriority::eUI, 0, ETaskPauseType::eGame,
		false, false
	);
	mpFrameSeparator->SetSize(TOUKI_FRAME_SIZE_X, TOUKI_FRAME_SIZE_Y);
}

// �f�X�g���N�^
CToukiGauge::~CToukiGauge()
{
	SAFE_DELETE(mpFrameImage);
	SAFE_DELETE(mpBarImage);
	SAFE_DELETE(mpFrameSeparator);
}

// �X�V����
void CToukiGauge::Update()
{
	// �Q�[�W�̃t���[���ƃo�[�̈ʒu��ݒ�
	mpFrameImage->SetPos(mPosition);
	mpFrameSeparator->SetPos(mPosition);
	mpBarImage->SetPos(mPosition + CVector2(TOUKI_FRAME_BORDER, TOUKI_FRAME_BORDER));

	// �Q�[�W�̃o�[�̃T�C�Y���ő�l�ƌ��ݒl���狁�߂�
	CVector2 size = CVector2(TOUKI_BARSIZE_X, TOUKI_BARSIZE_Y);
	float percent = Math::Clamp01((float)mValue / mMaxValue);
	size.X(TOUKI_BARSIZE_X * percent);
	mpBarImage->SetSize(size);

	// HP�o�[�̊����Ńo�[�̐F��ύX
	CColor color;
	// �F�ɐݒ�
	color = CColor::red;

	// �o�[�ɐF��ݒ�
	mpBarImage->SetColor(color);

	mpFrameImage->Update();
	mpBarImage->Update();
	mpFrameSeparator->Update();
}

void CToukiGauge::Render()
{
	mpFrameImage->Render();
	mpBarImage->Render();
	mpFrameSeparator->Render();
}