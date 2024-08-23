#include "CDragon.h"
#include "Global.h"
#include "CPlayer.h"
#include "Maths.h"
#include "CSPFlamethrower.h"
#include "CColliderLine.h"

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
	case 6:// �󒆃A�C�h������
		UpdateSpAttack_Step6();
		break;
	case 7:// �󒆈ړ�����3
		UpdateSpAttack_Step7();
		break;
	case 8:// ��������
		UpdateSpAttack_Step8();
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
		SetAnimationSpeed(0.45f);
		if (!mpFlySE->IsPlaying())
		{
			mpFlySE->PlayLoop(-1, true, 0.2f);
		}
		else
		{
			mpFlySE->SetBaseVolume(0.2f);
		}
		// �h���S������s���i�s�����Ɍ����Ȃ��o�O���������邽��
		// �O�̂��߂����ł�mBatteleStep��0�ɂ��Ă���
		mBatteleStep = 0;
	}

	// ���[�V�����u���[���|���n�߂�t���[���ȏ�̎�
	// ���[�V�����u���[���|����
	if (SCREAMBLUR_START <= GetAnimationFrame() &&
		GetAnimationFrame() <= SCREAMBLUR_START + 10.0f)
	{
		// ���[�V�����u���[���|���Ă���Œ��ł���΁A
		// �V�������[�V�����u���[���|�������Ȃ�
		if (mMotionBlurRemainTime <= 0.0f)
		{
			System::SetEnableMotionBlur(true);
			mMotionBlurRemainTime = MOTION_BLUR_TIME;
			// SE���Đ�
			CSound* screamSE = CResourceManager::Get<CSound>("Scream");
			screamSE->Play(0.4f);
		}
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
		SetAnimationSpeed(0.6f);
		mpFlySE->SetVolume(0.0f);

		// ���̃X�e�b�v�ňړ�����ړI�n��ݒ�
		CVector pPos = CPlayer::Instance()->Position();
		CVector fPos = gField->Position();
		CVector FP = pPos - fPos;
		FP.Y(0.0f);
		// �ړI�n�̍��W
		CVector targetPos = FP.Normalized() * FIELD_RADIUS * 0.7;
		targetPos.Y(0.0f);

		// �ړI�n�܂ł̃x�N�g����ݒ�
		CVector dPos = Position();
		dPos.Y(0.0f);
		CVector targetVec = (targetPos - dPos).Normalized();

		// �ړ�����ړI�n��ۑ�
		mSaveDestination = targetPos;
		mSaveVec = targetVec;
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
	mMoveSpeed += mSaveVec * 3.3f;

	// �d�͂ŗ����Ȃ��悤�ɒ���
	mMoveSpeed += CVector(0.0f, GRAVITY, 0.0f);
}

// �󒆈ړ�����2
// �ړI�n�܂ŊO�����ړ�
void CDragon::UpdateSpAttack_Step3()
{
	float dist = FIELD_RADIUS * 0.8f;
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

	// �ړI�n�ɒ������玟�̃X�e�b�v�Ɉڍs
	float targetLength = (mSaveDestination - myPos).Length();
	if (targetLength <= 130.0f)
	{
		mSpAttackStep++;
		ChangeAnimation(EDragonAnimType::eFlyIdle);
		SetAnimationSpeed(0.31f);
		mSaveDestination = CVector::zero;
		mpFlySE->SetVolume(0.2f);
	}
	// �f�o�b�O�\���p
	GetAngle();
}

// �󒆃A�C�h���y�э�������
void CDragon::UpdateSpAttack_Step4()
{
	CVector dPos = Position();
	CVector fPos = gField->Position();
	CVector FD = (fPos - dPos).Normalized();
	FD.Y(0.0f);
	// �X�e�[�W�̒��S�Ɍ�����
	mMoveSpeed = FD * 0.0000000001;

	mMoveSpeed -= CVector(0.0f, 0.35f, 0.0f);

	// 2�b�o�߂Ŏ��̃X�e�b�v�ֈڍs
	mElapsedTime += Time::DeltaTime();
	if (mElapsedTime >= 2.0f)
	{
		if (IsAnimationFinished())
		{
			mElapsedTime = 0.0f;
			mSpAttackStep++;
			ChangeAnimation(EDragonAnimType::eFlyFlame);
			SetAnimationSpeed(0.2f);
			mpFlySE->SetVolume(0.0f);
			mIsFlyBreath = true;
		}
	}
}

// �󒆃u���X�U��
void CDragon::UpdateSpAttack_Step5()
{
	// �d�͂ŗ����Ȃ��悤�ɒ���
	mMoveSpeed += CVector(0.0f, GRAVITY, 0.0f);

	// �u���X���ˏ���
	if (15.0f <= GetAnimationFrame() &&
		!mpSpFlamethrower->IsThrowing())
	{
		mpSpFlamethrower->Start();
		// �u���X�U�����I�������ɂ��Đ�����Ȃ��悤��
		// �Đ��t���[�����w��
		if (50.0f > GetAnimationFrame())
		{
			CSound* flybreathSE = CResourceManager::Get<CSound>("FlyBreath");
			flybreathSE->Play(0.35f);
		}
	}

	// �u���X��~����
	if (77.0f <= GetAnimationFrame() &&
		mpSpFlamethrower->IsThrowing())
	{
		mpSpFlamethrower->Stop();
	}

	// �U�����I�������A���̃X�e�b�v�ֈڍs
	if (IsAnimationFinished())
	{
		mSpAttackStep++;
		ChangeAnimation(EDragonAnimType::eFlyIdle);
		SetAnimationSpeed(0.31f);
		mIsFlyBreath = false;
		if (mpSpFlamethrower->IsThrowing())
		{
			mpSpFlamethrower->Stop();
		}
		mpFlySE->SetVolume(0.2f);
	}
}

// �󒆃A�C�h������
void CDragon::UpdateSpAttack_Step6()
{
	mElapsedTime += Time::DeltaTime();
	// 4�b�o�߂Ŏ��̃X�e�b�v�ֈڍs
	if (mElapsedTime >= 4.0f)
	{
		if (IsAnimationFinished())
		{
			mElapsedTime = 0.0f;
			mSpAttackStep++;
			ChangeAnimation(EDragonAnimType::eFlyForward);
			SetAnimationSpeed(0.6f);
			mpFlySE->SetVolume(0.0f);
		}
	}
	// �d�͂ŗ����Ȃ��悤�ɒ���
	mMoveSpeed += CVector(0.0f, GRAVITY, 0.0f);
}

// �󒆈ړ�����3
void CDragon::UpdateSpAttack_Step7()
{
	// �X�e�[�W�����ֈړ�����
	CVector fPos = gField->Position();
	CVector dPos = Position();
	CVector FD = fPos - dPos;
	FD.Y(0.0f);

	// ���g����X�e�[�W�̒��S�܂ł̋������擾
	float targetLength = FD.Length();
	mMoveSpeed += FD.Normalized() * 3.0f;

	// �d�͂ŗ����Ȃ��悤�ɒ���
	mMoveSpeed += CVector(0.0f, GRAVITY, 0.0f);

	// �X�e�[�W�̒��S�܂ňړ�������
	// ���̃X�e�b�v�ֈڍs����
	if (targetLength <= 30.0f)
	{
		mSpAttackStep++;
		ChangeAnimation(EDragonAnimType::eLand);
		SetAnimationSpeed(0.325f);
		mpColliderLine->Position(mpColliderLine->Position() + CVector(0.0f, 20.0f, 0.0f));
		mpFlySE->SetVolume(0.2f);
	}
}

// ��������
void CDragon::UpdateSpAttack_Step8()
{
	// ����������A�퓬��Ԃֈڍs
	if (IsAnimationFinished())
	{
		mSpAttackStep = 0;
		mBatteleStep = 0;
		mState = EState::eBattle;
		ChangeAnimation(EDragonAnimType::eIdle1);
		SetAnimationSpeed(0.4f);
		mpColliderLine->Position(CVector::zero);
		mpFlySE->SetVolume(0.0f);

		// ������ɋ��ݒl�����܂��Ă���ꍇ�A
		// ���݃��[�V�������s���Ă��܂����߁A���ݒl�𔼕��ɂ���
		mFearValue /= 2;
	}

	mMoveSpeed -= CVector(0.0f, GRAVITY*3, 0.0f);
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
	targetPos.Y(0.0f);

	// �ړI�l��ۑ�
	return targetPos;
}

// �O���̎��g�̂���ʒu�̒��S����݂��p�x���擾
float CDragon::GetAngle() const
{
	// 0�x�̃x�N�g��
	CVector zeropos = CVector::zero;
	zeropos.X(cosf(0.0f));
	zeropos.Z(sinf(0.0f));

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
			//CDebugPrint::Print("���ی�\n");
			//CDebugPrint::Print("�h���S���̌��݂̊p�x : %.1f\n", angle);
		}
		else // Z���}�C�i�X��
		{
			// ��l�ی�
			//CDebugPrint::Print("��l�ی�\n");
			angle = 180.0f + (180.0f - angle);
			//CDebugPrint::Print("�h���S���̌��݂̊p�x : %.1f\n", angle);
		}
	}
	else // X���}�C�i�X��
	{
		if (targetPos.Z() >= 0)
		{
			// ���ی�
			//CDebugPrint::Print("���ی�\n");
			//CDebugPrint::Print("�h���S���̌��݂̊p�x : %.1f\n", angle);
		}
		// Z���}�C�i�X��
		else
		{
			// ��O�ی�
			//CDebugPrint::Print("��O�ی�\n");
			angle = 180.0f + (180.0f - angle);
			//CDebugPrint::Print("�h���S���̌��݂̊p�x : %.1f\n", angle);
		}
	}
	return angle;
}

// �󒆃u���X�����ǂ���
bool CDragon::IsFlyBreath() const
{
	return mIsFlyBreath;
}