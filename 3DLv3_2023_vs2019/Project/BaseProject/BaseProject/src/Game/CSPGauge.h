#pragma once
#include "CUIGauge.h"
class CImage;

// �X�^�~�i�Q�[�W�Ǘ��N���X
class CSPGauge : public CUIGauge
{
private:
	CImage* mpSPFrameImage;		// SP�Q�[�W�̃t���[���̃C���[�W
	CImage* mpSPBarImage;		// SP�Q�[�W�̃o�[�C���[�W

public:
	// �R���X�g���N�^
	CSPGauge();
	// �f�X�g���N�^
	~CSPGauge();

	// �X�V
	void Update() override;
};