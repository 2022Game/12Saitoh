#pragma once
#include "CUIGauge.h"

#define TOUKI_FRAME_SIZE_X 256.0f
// �t���[���̏c�̃T�C�Y
#define TOUKI_FRAME_SIZE_Y 20.0f
// �t���[���̉��̕�
#define TOUKI_FRAME_BORDER 3.2f
// �o�[�̉��̃T�C�Y
#define TOUKI_BARSIZE_X (TOUKI_FRAME_SIZE_X - TOUKI_FRAME_BORDER * 1.6f)
// �o�[�̏c�̃T�C�Y
#define TOUKI_BARSIZE_Y (TOUKI_FRAME_SIZE_Y - TOUKI_FRAME_BORDER * 1.6f)

// ���C�Q�[�W�Ǘ��N���X
class CToukiGauge : public CUIGauge
{
private:
	CImage* mpFrameImage;	// HP�Q�[�W�̃t���[���C���[�W
	CImage* mpBarImage;		// HP�Q�[�W�̃o�[�C���[�W

public:
	// �R���X�g���N�^
	CToukiGauge();
	// �f�X�g���N�^
	~CToukiGauge();

	// �X�V
	void Update() override;
};