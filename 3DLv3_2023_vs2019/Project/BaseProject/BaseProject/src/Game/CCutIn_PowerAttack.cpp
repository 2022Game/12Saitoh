#include "CCutIn_PowerAttack.h"
#include "Maths.h"

// �R���X�g���N�^
CCutIn_PowerAttack::CCutIn_PowerAttack()
	: mCutInStep(0)
	, mElapsedTime(0.0f)
	, mCenterPos(0.0f, 0.0f, 0.0f)
	, mStartAngleY(0.0f)
	, mStartAngleX(0.0f)
	, mStartSideVec(0.0f, 0.0f, 0.0f)
{
}

// �f�X�g���N�^
CCutIn_PowerAttack::~CCutIn_PowerAttack()
{
}

// �J�b�g�C���J�n
bool CCutIn_PowerAttack::Start()
{
	// �x�[�X�̃J�b�g�C���J�n����
	if (!CCutInCamera::Start()) return false;

	// �J�b�g�C���Ɏg�p����ϐ��Ȃǂ̏�����
	mCutInStep = 0;
	mElapsedTime = 0.0f;

	return true;
}

// �J�b�g�C���I��
bool CCutIn_PowerAttack::End()
{
	// �x�[�X�̃J�b�g�C���I������
	if (!CCutInCamera::End()) return false;

	return true;
}

// �J�b�g�C���ɕK�v�ȏ���ݒ�
void CCutIn_PowerAttack::Setup(CObjectBase* obj)
{
	// �ݒ肳�ꂽ�I�u�W�F�N�g�̈ʒu�𒆐S�n�_�ɐݒ�
	mCenterPos = obj->Position();
	// �ݒ肳�ꂽ�I�u�W�F�N�g�̌��݂̌�����Y���̊p�x���擾
	mStartAngleY = obj->EulerAngles().Y();
	// �ݒ肳�ꂽ�I�u�W�F�N�g�̌��݂̌�����X���̊p�x���擾
	mStartAngleX = obj->EulerAngles().X();
	
	// �ݒ肳�ꂽ�I�u�W�F�N�g�̉������x�N�g�����擾
	mStartSideVec = obj->VectorX();
}

#define CUTIN_TIME 0.4f		// �p������
#define START_ANGLE 90.0f	// �J�n����Y����]�p�x
#define END_ANGLE 115.0f	// �I������Y����]�p�x
#define START_ANGLEX 0.0f	// �J�n����X����]�p�x
#define END_ANGLEX -20.0f	// �I������X����]�p�x
#define START_Y 10.0f		// �J�n���̃J�����̍���
#define END_Y 10.0f			// �I�����̃J�����̍���
#define START_X 0.0f		// �J�n���̃J�����̉�����
#define END_X -3.0f			// �I�����̃J�����̉�����
#define START_DIST 15.0f	// �J�n���̉�ʊg�k
#define END_DIST 14.0f		// �I�����̉�ʊg�k
#define WAIT_TIME 0.4f	// �҂�����

// �X�e�b�v0 �J��������]
void CCutIn_PowerAttack::CutInStep0()
{
	if (mElapsedTime < CUTIN_TIME)
	{
		// �J�b�g�C���̐i�s�x(0.0 �` 1.0)
		float per = mElapsedTime / CUTIN_TIME;

		// �J�b�g�C���̐i�s�x�ɉ����č�����⊮
		CVector offsetPos = CVector::zero;
		float offsetX = Math::Lerp(START_X, END_X, per);
		offsetPos += mStartSideVec * offsetX;
		float offsetY = Math::Lerp(START_Y, END_Y, per);
		offsetPos.Y(offsetY);

		// �J�b�g�C���̐i�s�x�ɉ����ĉ�]�p�x��⊮
		// X��
		float startAngX = mStartAngleX + START_ANGLEX;
		float endAngX = mStartAngleX + END_ANGLEX;
		float angleX = Math::Lerp(startAngX, endAngX, per);
		float radAngX = Math::DegreeToRadian(angleX);
		// Y��
		float startAngY = mStartAngleY + START_ANGLE;
		float endAngY = mStartAngleY + END_ANGLE;
		float angleY = Math::Lerp(startAngY, endAngY, per);
		float radAngY = Math::DegreeToRadian(angleY);

		// �J�b�g�C���̐i�s�x�ɉ����ĉ�ʊg�k�̕⊮
		float dist = Math::Lerp(START_DIST, END_DIST, per);

		// �J�����ʒu�̌v�Z
		mAt = mCenterPos + offsetPos;
		mEye = mAt + CVector(cosf(radAngX) * cosf(radAngY), sinf(radAngX), cosf(radAngX) * sinf(radAngY)) * dist;

		// �o�ߎ��Ԃ̍X�V
		mElapsedTime += Time::DeltaTime();
	}
	else
	{
		// �J�b�g�C���I�����̈ʒu�ƌ���
		CVector offsetPos = CVector::zero;
		offsetPos += mStartSideVec * END_X;
		offsetPos.Y(END_Y);

		float radAngX = Math::DegreeToRadian(END_ANGLEX);
		float radAngY = Math::DegreeToRadian(mStartAngleY + END_ANGLE);
		float dist = END_DIST;

		mAt = mCenterPos + offsetPos;
		mEye = mAt + CVector(cosf(radAngX) * cosf(radAngY), sinf(radAngX), cosf(radAngX) * sinf(radAngY)) * dist;

		// �o�ߎ��Ԃ����������A���̃X�e�b�v�֐i��
		mCutInStep++;
		mElapsedTime = 0.0f;
	}
}

// �X�e�b�v1 �J�b�g�C���I�����̑҂�
void CCutIn_PowerAttack::CutInStep1()
{
	if (mElapsedTime < WAIT_TIME)
	{
		mElapsedTime += Time::DeltaTime();
	}
	else
	{
		mCutInStep = 0;
		mElapsedTime = 0.0f;
		End();
	}
}

// �X�V
void CCutIn_PowerAttack::Update()
{
	// �J�b�g�C���Đ����o�Ȃ���΁A�������Ȃ�
	if (!mIsPlaying) return;

	switch (mCutInStep)
	{
	case 0:
		CutInStep0();
		break;
	case 1:
		CutInStep1();
		break;
	}

	// �J�����̍X�V
	CCamera::Update();
}