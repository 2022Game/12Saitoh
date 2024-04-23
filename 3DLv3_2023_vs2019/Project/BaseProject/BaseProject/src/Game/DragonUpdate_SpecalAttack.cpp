#include "CDragon.h"
#include "Global.h"
#include "CPlayer.h"
#include "Maths.h"

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
	case 2:// �ړI�n��ݒ�
		UpdateSpAttack_Step2();
		break;
	case 3:// �󒆈ړ�����1
		UpdateSpAttack_Step3();
		break;
	case 4:// �󒆈ړ�����2
		UpdateSpAttack_Step4();
		break;
	case 5:// �󒆃u���X�U��
		UpdateSpAttack_Step5();
		break;
	case 6:// �󒆈ړ�����3
		UpdateSpAttack_Step6();
		break;
	case 7:// ��������
		UpdateSpAttack_Step7();
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
	// ����
	mMoveSpeed += CVector(0.0f, GRAVITY + 0.7f, 0.0f);

	// �������I��������A���̏����ֈڍs����
	if (IsAnimationFinished())
	{
		mSpAttackStep++;
		ChangeAnimation(EDragonAnimType::eFlyForward);

		// ���̃X�e�b�v�ňړ�����ړI�n��ݒ�
		CVector pPos = CPlayer::Instance()->Position();
		CVector fPos = gField->Position();
		CVector FP = pPos - fPos;
		FP.Y(0.0f);
		// �ړI�n�̍��W
		CVector targetPos = FP.Normalized() * FIELD_RADIUS * 0.7;
		targetPos.Y(0.0f);

		// �ړ�����ړI�n��ۑ�
		mSaveDestination = targetPos;
		mSaveVec = targetPos.Normalized();
	}
}

// �󒆈ړ�����1
// �O���܂ňړ�
void CDragon::UpdateSpAttack_Step2()
{
	CVector myPos = Position();
	myPos.Y(0.0f);
	// �O���܂ł̋�����100�ȉ��Ȃ�A
	// ���̈ړ��X�e�b�v�Ɉڍs����
	float targetLength = (mSaveDestination - myPos).Length();
	if (targetLength <= 100.0f)
	{
		// �p�x�̐ݒ�
		mAngle = GetAngle();

		// �ړI�n�̍X�V
		mSaveDestination = GetDestination();
		mSaveVec = CVector::zero;
		mSpAttackStep++;
	}

	// �ړ�����
	mMoveSpeed += mSaveVec * 3.0f;

	// �d�͂ŗ����Ȃ��悤�ɒ���
	mMoveSpeed += CVector(0.0f, GRAVITY, 0.0f);
}

// �󒆈ړ�����2
// �ړI�n�܂ŊO�����ړ�
void CDragon::UpdateSpAttack_Step3()
{
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

	// �f�o�b�O�\���p
	GetAngle();
}

// �󒆃A�C�h���y�э�������
void CDragon::UpdateSpAttack_Step4()
{

}

// �󒆃u���X�U��
void CDragon::UpdateSpAttack_Step5()
{

}

// �󒆈ړ�����3
void CDragon::UpdateSpAttack_Step6()
{

}

// ��������
void CDragon::UpdateSpAttack_Step7()
{
	// ����������A�퓬��Ԃֈڍs
	if (IsAnimationFinished())
	{
		mSpAttackStep = 0;
		mState = EState::eBattle;
		ChangeAnimation(EDragonAnimType::eIdle1);
	}
}

// �󒆃u���X�U�����s���ʒu(�ړI�n)�̐ݒ�
CVector CDragon::GetDestination() const
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
	return targetPos;
}

// �O���̎��g�̂���ʒu�̒��S����݂��p�x���擾
float CDragon::GetAngle() const
{
	float dist = FIELD_RADIUS;
	// 0�x�̃x�N�g��
	CVector zeropos = CVector::zero;
	zeropos.X(cosf(Math::DegreeToRadian(0.0f)) * dist);
	zeropos.Z(sinf(Math::DegreeToRadian(0.0f)) * dist);

	// ���_���玩�g�̊p�x�̃x�N�g��
	CVector dPos = Position();
	CVector fPos = gField->Position();
	CVector DP = dPos - fPos;
	DP.Y(0.0f);
	CVector targetPos = DP.Normalized() * FIELD_RADIUS;

	// �p�x�̌v�Z
	targetPos.Normalized();
	float angle = Math::RadianToDegree(CVector::Angle(zeropos, targetPos));
	// X���v���X��
	if (targetPos.X() >= 0)
	{
		// Y���v���X��
		if (targetPos.Z() >= 0)
		{
			// ���ی�
			CDebugPrint::Print("���ی�\n");
			CDebugPrint::Print("�h���S���̌��݂̊p�x : %.1f\n", angle);
		}
		else // Z���}�C�i�X��
		{
			// ��l�ی�
			CDebugPrint::Print("��l�ی�\n");
			angle = 180.0f + (180.0f - angle);
			CDebugPrint::Print("�h���S���̌��݂̊p�x : %.1f\n", angle);
		}
	}
	else // X���}�C�i�X��
	{
		if (targetPos.Z() >= 0)
		{
			// ���ی�
			CDebugPrint::Print("���ی�\n");
			CDebugPrint::Print("�h���S���̌��݂̊p�x : %.1f\n", angle);
		}
		// Z���}�C�i�X��
		else
		{
			// ��O�ی�
			CDebugPrint::Print("��O�ی�\n");
			angle = 180.0f + (180.0f - angle);
			CDebugPrint::Print("�h���S���̌��݂̊p�x : %.1f\n", angle);
		}
	}
	return angle;
}