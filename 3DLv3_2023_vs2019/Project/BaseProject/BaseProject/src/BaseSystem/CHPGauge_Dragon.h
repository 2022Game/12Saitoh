#pragma once
#include "CUIGauge.h"
class CImage;

// �t���[���̉��̃T�C�Y
#define DRAGON_FRAME_SIZE_X 485.0f * 1.5
// �t���[���̏c�̃T�C�Y
#define DRAGON_FRAME_SIZE_Y 30.0f
// �t���[���̉��̕�
#define DRAGON_FRAME_BORDER 5.5f * 1.2
// �o�[�̉��̃T�C�Y
#define DRAGON_BARSIZE_X (DRAGON_FRAME_SIZE_X - DRAGON_FRAME_BORDER * 1.8)
// �o�[�̏c�̃T�C�Y
#define DRAGON_BARSIZE_Y (DRAGON_FRAME_SIZE_Y - DRAGON_FRAME_BORDER * 1.75)

// �X�^�~�i�Q�[�W�Ǘ��N���X
class CHPGauge_Dragon : public CUIGauge
{
private:
	CImage* mpHPFrameImage;	// HP�Q�[�W�̃t���[���C���[�W
	CImage* mpHPBarImage;	// HP�Q�[�W�̃o�[�C���[�W

public:
	// �R���X�g���N�^
	CHPGauge_Dragon();
	// �f�X�g���N�^
	~CHPGauge_Dragon();

	// �X�V
	void Update() override;
	void Render() override;
};