#pragma once
#include "CCutInCamera.h"

// �e�X�g�p�J�b�g�C��
class CCutInTest : public CCutInCamera
{
public:
	// �R���X�g���N�^
	CCutInTest();
	// �f�X�g���N�^
	~CCutInTest();

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

	int mCutInStep	;	// �J�b�g�C���̃X�e�b�v
	float mElapsedTime;	// �o�ߎ��Ԍv����

	CVector mCenterPos;	// �J�b�g�C���̒��S�n�_
	float mStartAngleY;	// �J�n�̃J������Y���p�x
	float mStartAngleX;	// �J�n�̃J������X���p�x
};