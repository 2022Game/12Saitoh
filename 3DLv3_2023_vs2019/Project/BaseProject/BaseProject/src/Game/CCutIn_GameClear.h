#pragma once
#include "CCutInCamera.h"

// �Q�[���N���A�p�J�b�g�C��
class CCutIn_GameClear : public CCutInCamera
{
public:
	// �R���X�g���N�^
	CCutIn_GameClear();
	// �f�X�g���N�^
	~CCutIn_GameClear();

	// �J�b�g�C���J�n
	bool Start() override;
	// �J�b�g�C���I��
	bool End() override;

	// �J�b�g�C���ɕK�v�ȏ���ݒ�
	void Setup(CObjectBase* obj);

	// �X�V
	void Update() override;
private:
	// �X�e�b�v0 �J��������]
	void CutInStep0();
	// �X�e�b�v1 �J�b�g�C���I�����̑҂�
	void CutInStep1();

	int mCutInStep;		// �J�b�g�C���̃X�e�b�v
	float mElapsedTime;	// �o�ߎ��Ԍv����

	CVector mCenterPos;		// �J�b�g�C���̒��S�n�_
	CVector mStartSideVec;	// �J�n���̃h���S���̉������̃x�N�g��
	float mStartAngleY;		// �J�n�̃J������Y���p�x
	float mStartAngleX;		// �J�n�̃J������X���p�x
};