#pragma once
#include "CUIGauge.h"
class CImage;

// �X�^�~�i�Q�[�W�Ǘ��N���X
class CHPGauge : public CUIGauge
{
private:
	CImage* mpHPFrameImage;	// HP�Q�[�W�̃t���[���̃C���[�W
	CImage* mpHPBarImage;	// HP�Q�[�W�̃o�[�C���[�W
	CImage* mpRedBarImage;	// �ԃQ�[�W�̃o�[�C���[�W

public:
	// �R���X�g���N�^
	CHPGauge();
	// �f�X�g���N�^
	~CHPGauge();

	// �ԃQ�[�W�̍X�V����
	void Update_RedBar();

	// �X�V
	void Update() override;
	void Render() override;
};