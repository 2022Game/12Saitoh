#pragma once
#include "CCutInCamera.h"

// �Q�[���I�[�o�[�p�J�b�g�C��
class CCutIn_GameOver : public CCutInCamera
{
public:
	// �R���X�g���N�^
	CCutIn_GameOver();
	// �f�X�g���N�^
	~CCutIn_GameOver();

	// �J�b�g�C���J�n
	bool Start() override;

	// �J�b�g�C���ɕK�v�ȏ���ݒ�
	void Setup(CObjectBase* obj);

	// �X�V
	void Update() override;
private:
	// �J��������]
	void CutIn();

	int mCutInStep;		// �J�b�g�C���̃X�e�b�v
	float mElapsedTime;	// �o�ߎ��Ԍv����

	CVector mCenterPos;		// �J�b�g�C���̒��S�n�_
	CVector mStartSideVec;	// �J�n���̃h���S���̉������̃x�N�g��
	float mStartAngleY;		// �J�n�̃J������Y���p�x
	float mStartAngleX;		// �J�n�̃J������X���p�x
};