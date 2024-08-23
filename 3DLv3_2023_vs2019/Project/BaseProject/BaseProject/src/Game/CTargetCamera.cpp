#include "CTargetCamera.h"

CTargetCamera* CTargetCamera::spTargetCamera = nullptr;

// �R���X�g���N�^
CTargetCamera::CTargetCamera()
	: CCamera(CVector::zero,CVector::zero,false)
	, mIsPlaying(false)
	, mEyePos(CVector::zero)
	, mAtPos(CVector::zero)
{
}

// �f�X�g���N�^
CTargetCamera::~CTargetCamera()
{

}

// �^�[�Q�b�g�J�����J�n
void CTargetCamera::Start()
{
	// ���ɃJ�������I���ɂȂ��Ă�����J�n���Ȃ�
	if (IsPlaying()) return;

	// �Đ����̃J�����Ɏ��g��ݒ�
	spTargetCamera = this;
	mIsPlaying = true;
	// ���݂̃J���������g�ɐݒ�
	SetCurrent(true);
}

// �^�[�Q�b�g�J�����I��
void CTargetCamera::End()
{
	// �Đ����Ŗ�����Β�~���Ȃ�
	if (!IsPlaying()) return;

	// �Đ����̃J�b�g�C���J����������
	spTargetCamera = nullptr;
	mIsPlaying = false;

	// ���݂̃J�������������
	SetCurrent(false);
}

void CTargetCamera::Update_Set(CObjectBase* Eyeobj, CObjectBase* Atobj)
{
	// �^�[�Q�b�g�J�����ɕK�v�ȏ����擾
	mEyePos = Eyeobj->Position();
	mAtPos = Atobj->Position();
}

bool CTargetCamera::IsPlaying() const
{
	return mIsPlaying;
}

void CTargetCamera::Update()
{
	// �J�������Đ����Ŗ�����΍X�V���Ȃ�
	if (!IsPlaying()) return;
	// ���_
	CVector eye = mEyePos - mAtPos;
	eye.Y(0.0f);
	eye.Normalize();
	eye *= 40.0f;
	eye.Y(10.0f);
	mEye = mEyePos + eye;

	// �����_
	// Y���W�͊�{�I��0�Ȃ̂ŏ����グ��
	mAt = mAtPos + CVector(0.0f, 35.0f, 0.0f);

	// �ݒ肳��Ă���R���C�_�[�ƏՓ˂���ꍇ�́A
	// �J�����̈ʒu�������o��
	ApplyCollision();

	// ���_�A�����_�A��x�N�g������e�s����X�V
	LookAt(mEye, mAt, mUp, false);

}