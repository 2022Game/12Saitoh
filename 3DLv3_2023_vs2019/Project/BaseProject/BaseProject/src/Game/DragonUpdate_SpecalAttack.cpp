#include "CDragon.h"
#include "Global.h"
#include "CPlayer.h"

// �K�E�Z(�󒆃u���X�U��)����
void CDragon::UpdateSpecalAttack()
{
	mMoveSpeed.X(0.0f);
	mMoveSpeed.Y(0.0);
	mMoveSpeed.Z(0.0f);

	// �U���i�K�ɉ����āA���������s
	switch (mSpAttackStep)
	{
	case 0:// ���K����
		UpdateSpAttack_Step0();
		break;
	case 1:// ����
		UpdateSpAttack_Step1();
		break;
	case 2:
		UpdateSpAttack_Step2();
		break;
	case 3:// �󒆈ړ�
		UpdateSpAttack_Step3();
		break;
	case 4:// �󒆃u���X�U��
		UpdateSpAttack_Step4();
		break;
	case 5:// �󒆈ړ�
		UpdateSpAttack_Step5();
	case 6:// ��������
		UpdateSpAttack_Step6();
		break;
	}

}

// ���K����
void CDragon::UpdateSpAttack_Step0()
{
	// ���K���I��������A���̏����ֈڍs����
	if (IsAnimationFinished())
	{
		mSpAttackStep++;
		ChangeAnimation(EDragonAnimType::eTakeOff);
	}
}

// ��������
void CDragon::UpdateSpAttack_Step1()
{
	mMoveSpeed += CVector(0.0f, GRAVITY + 0.7f, 0.0f);
	// �������I��������A���̏����ֈڍs����
	if (IsAnimationFinished())
	{
		mSpAttackStep++;
		ChangeAnimation(EDragonAnimType::eFlyForward);
	}
}


void CDragon::UpdateSpAttack_Step2()
{
	// �ړI�n�̐ݒ�
	CVector fieldPos = gField->Position();
	CVector playerPos = CPlayer::Instance()->Position();
	CVector playerDir = playerPos - fieldPos;
	playerDir.Y(0.0f);
	CVector targetDir = -playerDir;
	// �ړI�n
	CVector targetPos = targetDir.Normalized() * FIELD_RADIUS * 0.8;

	// �ړI�l��ۑ�
	mSaveVec = targetPos;
	mSpAttackStep++;
}

#include "Maths.h"
// �󒆈ړ�����1
void CDragon::UpdateSpAttack_Step3()
{
	// �ړI�n�܂ł̃x�N�g�����擾
	CVector myPos = Position();
	myPos.Y(0.0f);
	//CVector targetVec = (mSaveVec - myPos).Normalized();

	// 0�x�̊p�x�܂ł̃x�N�g�����擾
	float dist = FIELD_RADIUS * 0.6f;
	CVector pos = CVector::zero;
	pos.X(cosf(Math::DegreeToRadian(mAngle)) * dist);
	pos.Z(sinf(Math::DegreeToRadian(mAngle)) * dist);

	float targetLength = (pos - myPos).Length();
	if (targetLength <= 100.0f)
	{
		mSpAttackStep++;
	}

	CVector posVec = pos.Normalized();
	mMoveSpeed += posVec * 3.0f;

	// �ړI�n�܂ňړ�����
	//mMoveSpeed += targetVec * 1.0 ;
	// �d�͂ŗ����Ȃ��悤�ɒ���
	mMoveSpeed += CVector(0.0f, GRAVITY, 0.0f);
}

// �󒆃u���X�U������
void CDragon::UpdateSpAttack_Step4()
{
	// ���͉��ŋ󒆈ړ�����������
	float dist = FIELD_RADIUS * 0.9f;
	CVector pos = CVector::zero;
	pos.X(cosf(Math::DegreeToRadian(mAngle)) * dist);
	pos.Z(sinf(Math::DegreeToRadian(mAngle)) * dist);

	CVector myPos = Position();
	myPos.Y(0.0f);
	CVector targetVec = (pos - myPos).Normalized();

	mMoveSpeed += targetVec * 4.0f;
	mAngle += 0.6;
	if (mAngle > 360.0f)
	{
		mAngle = 0.0f;
	}
	// �d�͂ŗ����Ȃ��悤�ɒ���
	mMoveSpeed += CVector(0.0f, GRAVITY, 0.0f);
}

// �󒆈ړ�����2
void CDragon::UpdateSpAttack_Step5()
{

}

// ��������
void CDragon::UpdateSpAttack_Step6()
{
	// ����������A�퓬��Ԃֈڍs
	if (IsAnimationFinished())
	{
		mSpAttackStep = 0;
		mState = EState::eBattle;
		ChangeAnimation(EDragonAnimType::eIdle1);
	}
}
