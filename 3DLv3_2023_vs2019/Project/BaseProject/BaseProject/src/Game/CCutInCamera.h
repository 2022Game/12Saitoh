#pragma once
#include "CCamera.h"

// �J�b�g�C���̎��̃J�����x�[�X�N���X
class CCutInCamera : public CCamera
{
public:
	// �R���X�g���N�^
	CCutInCamera();
	//�f�X�g���N�^
	virtual ~CCutInCamera();

	// �J�b�g�C���J�n
	virtual bool Start();
	// �J�b�g�C���J�n(�|�[�Y����)
	virtual bool Start_NoPause();
	// �J�b�g�C���I��
	virtual bool End();

	// �J�b�g�C���Đ������ǂ���
	bool IsPlaying() const;

	// �X�V
	void Update() override;

protected:
	// ���ݍĐ����̃J�b�g�C���J����
	static CCutInCamera* spCutInCamera;
	bool mIsPlaying;	// �J�b�g�C���Đ������ǂ���
};