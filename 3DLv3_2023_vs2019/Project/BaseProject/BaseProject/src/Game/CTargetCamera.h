#pragma once
#include "CCamera.h"
// �^�[�Q�b�g�J����
class CTargetCamera : public CCamera
{
public:
	// �R���X�g���N�^
	CTargetCamera();
	// �f�X�g���N�^
	~CTargetCamera();

	// �^�[�Q�b�g�J�����J�n
	void Start();
	// �^�[�Q�b�g�J�����I��
	void End();

	/// <summary>
	/// �^�[�Q�b�g�J�����ɕK�v�ȏ����擾
	/// </summary>
	/// <param name="Eyeobj">���_�ɐݒ肷��I�u�W�F�N�g</param>
	/// <param name="Atobj">�^�[�Q�b�g�ɐݒ肷��I�u�W�F�N�g</param>
	void Update_Set(CObjectBase* Eyeobj, CObjectBase* Atobj);

	// �^�[�Q�b�g�J�����Đ������ǂ���
	bool IsPlaying() const;

	// �X�V����
	void Update() override;
private:
	// ���ݍĐ����̃^�[�Q�b�g�J����
	static CTargetCamera* spTargetCamera;
	bool mIsPlaying;	// �Đ������ǂ���

	CVector mEyePos;	// ���_�I�u�W�F�N�g�̈ʒu
	CVector mAtPos;		// �^�[�Q�b�g�I�u�W�F�N�g�̈ʒu
};