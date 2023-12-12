#pragma once
#include "CUIBase.h"
class CImage;

// HP�ESP�t���[���摜�̃t�@�C���p�X
#define FRAME_IMAGE "UI\\Gauge_Frame.png"
// HP�ESP�Q�[�W�o�[�摜�̃t�@�C���p�X
#define GAUGE_BAR "UI\\GaugeBar.png"

// �t���[���̉��̃T�C�Y
#define FRAME_SIZE_X (256.0f) * 1.5
// �t���[���̏c�̃T�C�Y
#define FRAME_SIZE_Y (20.0f) * 1.5
// �t���[���̉��̕�
#define FRAME_BORDER (5.0f)
// �o�[�̉��̃T�C�Y
#define BARSIZE_X (FRAME_SIZE_X - FRAME_BORDER * 1.9)
// �o�[�̏c�̃T�C�Y
#define BARSIZE_Y (FRAME_SIZE_Y - FRAME_BORDER * 1.9)

// UI�̃Q�[�W�\���N���X
class CUIGauge : public CUIBase
{
public:
	// �R���X�g���N�^
	CUIGauge();
	// �f�X�g���N�^
	~CUIGauge();

	// �ő�l��ݒ�
	void SetMaxValue(int value);
	// ���ݒl��ݒ�
	void SetValue(int value);
	// ���ݒl��ݒ�
	void SetValueF(float valuef);

	// �X�V
	void Update() override;

protected:
	int mMaxValue;			// �|�C���g�̍ő�l
	int mValue;				// �|�C���g�̌��ݒl
	float mValueF;			// �|�C���g�̌��ݒl(float�^)
};