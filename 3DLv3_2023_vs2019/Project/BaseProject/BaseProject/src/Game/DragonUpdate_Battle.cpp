#include "CDragon.h"
#include "Maths.h"
#include "Time.h"
#include "CColliderLine.h"
#include "CPlayer.h"
#include "CFlamethrower.h"

// �퓬���̍X�V����
void CDragon::UpdateBattle()
{
	mMoveSpeed.X(0.0f);
	mMoveSpeed.Z(0.0f);

	// �퓬�̒i�K�ɂ���ď�����ύX
	switch (mBatteleStep)
	{
	case 0:// �퓬���̃A�C�h������
		UpdateBattele_Idle();
		// �{���Ԃւ̈ڍs����
		if (mChangeAngry) ChangeAngry();
		break;
	case 1:// �퓬���̒ǐՏ���
		UpdateBattle_Chase();
		break;
	case 2:// �U������
		UpdateAttack();
		break;
	}

	// �{���Ԃ��ǂ���
	if (mIsAngry)
	{
		// �{���ԂɂȂ��Ă���̎��Ԃ��擾
		mAngryElapsedTime += Time::DeltaTime();

		// �{���ԂɂȂ��Ă���1���o�߂œ{���Ԃ�����
		if (mAngryElapsedTime >= 60.0f)
		{
			mIsAngry = false;
			mAngryElapsedTime = 0.0f;
		}
	}
}

// �퓬���̃A�C�h������
void CDragon::UpdateBattele_Idle()
{
	// �K�E�Z�U���ւ̈ڍs����
	// HP��50%�ȉ����A��x���K�E�Z�������Ă��Ȃ�
	if (GetHPPercent() <= HPPER_50 && mSpAttackNum == 0)
	{
		mSpAttackNum++;
		mState = EState::eSpAttack;
		ChangeAnimation(EDragonAnimType::eScream);
		SetAnimationSpeed(0.5f);
		return;
	}
	// HP��25%�ȉ��ɂȂ����������x����
	else if (GetHPPercent() <= HPPER_25 && mSpAttackNum == 1)
	{
		mSpAttackNum++;
		mState = EState::eSpAttack;
		ChangeAnimation(EDragonAnimType::eScream);
		SetAnimationSpeed(0.5f);
		return;
	}

	// �o�ߎ��Ԃ����Z����
	mElapsedTime += Time::DeltaTime();
	// 1.5�b���ƂɍU�����s��
	if (mElapsedTime >= 1.5f)
	{
		// �o�ߎ��Ԃ�������
		mElapsedTime = 0.0f;
		mRandSave = 0;
		mBatteleStep++;

		// �v���C���[�Ƃ̋������擾
		PlayerFromDistance();
		return;
	}

	// �ۑ��p�̕ϐ��ɐ������������������āB
	// �������������ɉ����ď��������s
	if (mRandSave == 0) mRandSave = BattleIdleRand();

	switch (mRandSave)
	{
	case 1: // ���̏�őҋ@���[�V�������s��
		ChangeAnimation(EDragonAnimType::eIdle1);
		SetAnimationSpeed(0.4f);
		break;

	case 2:// �v���C���[�̌��ֈړ�����
	{
		CVector playerPos = CPlayer::Instance()->Position();
		CVector enemyPos = Position();
		// ���g����v���C���[�܂ł̃x�N�g�����擾
		CVector EP = (playerPos - enemyPos).Normalized();
		EP.Y(0.0f);
		mMoveSpeed += EP * 0.3f;
		ChangeAnimation(EDragonAnimType::eWalk);
		SetAnimationSpeed(0.7f);
	}
		break;

	case 3:// �U�����s��

		// �e�ϐ���������
		mElapsedTime = 0.0f;
		mRandSave = 0;
		mBatteleStep++;
		// �v���C���[�Ƃ̋������擾
		PlayerFromDistance();
		break;
	}
}

// �퓬���̒ǐՏ���
void CDragon::UpdateBattle_Chase()
{
	CVector playerPos = CPlayer::Instance()->Position();
	CVector enemyPos = Position();

	// �����ɉ����ď������s��
	switch (mDistanceType)
	{
	case EDistanceType::eNear:// �ߋ���
	{
		CVector forward = VectorZ().Normalized();
		// ���g����v���C���[�܂ł̃x�N�g�����擾
		CVector EP = (playerPos - enemyPos).Normalized();
		EP.Y(0.0f);
		// ���ʕ����̃x�N�g���ƃv���C���[�܂ł̓��ς���p�x�����߂�
		float dotZ = forward.Dot(EP);
		// ���g����v���C���[�܂ł̃x�N�g���̒������擾
		float distance = (playerPos - enemyPos).Length();

		switch (mRandSave)
		{
		case 1:// ���݂��U���͈̔͂܂ňړ����čU�����s��
			// ���݂��U���͈͓̔��̏ꍇ
			if (distance <= 120.0f)
			{
				// ���݂��U�����s��
				ChangeAnimation(EDragonAnimType::eAttackMouth);
				SetAnimationSpeed(0.43);
				mBatteleStep++;
				mChaseElapsedTime = 0.0f;
			}
			else
			{
				// �U�����ł���͈͂܂ňړ�
				mMoveSpeed += EP * 0.6f;
				ChangeAnimation(EDragonAnimType::eWalk);
				SetAnimationSpeed(0.7f);
				// �ړ����Ԃ����Z
				mChaseElapsedTime += Time::DeltaTime();
			}
			// 5�b�ȏ�ړ������ꍇ�A�������U���Ɉڍs����
			if (mChaseElapsedTime >= 4.0f)
			{
				mChaseElapsedTime = 0.0f;
				mDistanceType = EDistanceType::eMedium;
			}
			break;

		case 2:// �o�b�N�X�e�b�v���s���A���̌�U���Ɉڍs

			// �o�b�N�X�e�b�v����
			if (AnimationIndex() == (int)EDragonAnimType::eBackStep)
			{
				Update_BackStep();
			}
			// �o�b�N�X�e�b�v���I��������
			if (IsAnimationFinished())
			{
				// �����𐶐����āA�U�������肷��
				int rand = Math::Rand(0, 1);
				if (rand == 0)
				{
					// �������U���ɐ؂�ւ�
					mDistanceType = EDistanceType::eMedium;
					mRandSave = 0;
				}
				else
				{
					// ���͈̔͂܂ňړ����čU�����s��
					mRandSave = 1;
				}

			}
			break;

		default:// �ŏ��Ɉ�x�������s
			// ���݂��U���͈̔͊O�������ꍇ
			if (distance >= 150.0f)
			{
				mRandSave = 1;
			}
			// ���݂��͈͓��������ꍇ
			else if ( 80.0f <= distance && distance <= 150.0f)
			{
				if (!(dotZ <= cosf(15 * M_PI / 180.0f)))
				{
					ChangeAnimation(EDragonAnimType::eAttackMouth);
					SetAnimationSpeed(0.43f);
					mBatteleStep++;
					mChaseElapsedTime = 0.0f;
				}
				else
				{
					mRandSave = 2;
					ChangeAnimation(EDragonAnimType::eBackStep);
					SetAnimationSpeed(0.5f);
				}
			}
			else// ����ȊO�̏ꍇ
			{
				mRandSave = 2;
				ChangeAnimation(EDragonAnimType::eBackStep);
				SetAnimationSpeed(0.5f);
			}
			break;
		}
		break;
	}
	case EDistanceType::eMedium:// ������
	{
		// ���g����v���C���[�܂ł̃x�N�g�����擾
		CVector EP = (playerPos - enemyPos).Normalized();
		EP.Y(0.0f);
		CVector forward = VectorZ().Normalized();
		// ���ʕ����̃x�N�g���ƃv���C���[�܂ł�
		// ���ς���p�x�����߂�
		float dotZ = forward.Dot(EP);

		// ���߂��p�x������p�x��
		// �������́A2�b�ȏ�o�߂ōU��
		if (mChaseElapsedTime >= 1.5f || !(dotZ <= cosf(10 * M_PI / 180.0f)))
		{
			// �����_���l�𐶐����āA�s���U����I��
			// ��ъ|����U�������߂ɂȂ�悤����
			switch (Math::Rand(0, 5))
			{
			case 0:
			case 1:// �u���X�U��
				ChangeAnimation(EDragonAnimType::eAttackFlame);
				SetAnimationSpeed(0.33f);
				mBatteleStep++;
				mChaseElapsedTime = 0.0f;
				break;
			default:// ��ъ|����U��
				ChangeAnimation(EDragonAnimType::eAttackHand);
				SetAnimationSpeed(0.5f);
				mBatteleStep++;
				mChaseElapsedTime = 0.0f;
				break;
			}
		}
		// ����͈͊O
		else
		{
			// �v���C���[�̕���������
			mMoveSpeed += EP * 0.2f;
			mChaseElapsedTime += Time::DeltaTime();
			ChangeAnimation(EDragonAnimType::eWalk);
			SetAnimationSpeed(0.7f);
		}

		break;
	}
	case EDistanceType::eFar:// ������
	{
		// �����ۑ��p�̕ϐ��������l�̏ꍇ�A
		// �����𐶐����đ������
		if (mRandSave == 0) mRandSave = Math::Rand(1, 2);

		switch (mRandSave)
		{
		case 1:
		case 2:// ���͈̔͂܂ňړ�
		{
			// ���g����v���C���[�܂ł̃x�N�g���̒������擾
			float distance = (playerPos - enemyPos).Length();

			// �ړ�
			if (distance >= 230.0f)
			{
				// ���g����v���C���[�܂ł̃x�N�g�����擾
				CVector EP = (playerPos - enemyPos).Normalized();
				EP.Y(0.0f);
				// ���͈̔͂܂ňړ�����
				mMoveSpeed += EP * 1.0f;
				ChangeAnimation(EDragonAnimType::eWalk);
				SetAnimationSpeed(0.7f);
				// �ړ����Ԃ����Z
				mChaseElapsedTime += Time::DeltaTime();
			}
			// ���͈̔͂܂ňړ�������
			else
			{
				// ������ۑ������ϐ���������
				mRandSave = 0;
				// �����_���l�𐶐����āA�U�������߂�
				int rand = Math::Rand(0, 1);
				// �ߋ���
				if (rand == 0) mDistanceType = EDistanceType::eNear;
				// ������
				else
				{
					mDistanceType = EDistanceType::eMedium;
					// �������̏����͈ړ��������̂ŁA�ړ����Ԃ�������
					mChaseElapsedTime = 0.0f;
				}
			}

			// 5�b�ȏ�ړ�������A�U�����s��
			if (mChaseElapsedTime >= 5.0f)
			{
				// ������ۑ������ϐ���������
				mRandSave = 0;
				// �u���X�U�����s��
				ChangeAnimation(EDragonAnimType::eAttackFlame);
				SetAnimationSpeed(0.33f);
				// �ړ����Ԃ̏�����
				mChaseElapsedTime = 0.0f;
				mBatteleStep++;
			}
		}
		break;
		}
	}
	}
}

// �U������
void CDragon::UpdateAttack()
{
	// �e�A�j���[�V�������Ƃɏ������s��
	switch (AnimationIndex())
	{
	case (int)EDragonAnimType::eScream:// ���K
		Update_Sceream();
		break;
	case (int)EDragonAnimType::eAttackMouth:// ���݂��U��
		Update_AttackMouth();
		break;
	case (int)EDragonAnimType::eAttackHand:// ��ъ|����U��
		Update_AttackHand();
		break;
	case (int)EDragonAnimType::eAttackFlame:// �n��u���X�U��
		Update_AttackFlame();
		break;
	}

	// �U�����I��������A�C�h���Ɉڍs����
	if (IsAnimationFinished())
	{
		mAttackStep = 0;
		mBatteleStep = 0;
		// �u���X�U�������Ă�����A�u���X���~�߂�
		if (mpFlamethrower->IsThrowing())mpFlamethrower->Stop();
	}
}

// ���K�U������
void CDragon::Update_Sceream()
{
	// ���K�p�̃R���C�_�̐؂�ւ�
	// �G�t�F�N�g�֘A�̏����Ȃǂ��s���\��
}

// ���݂��U������
void CDragon::Update_AttackMouth()
{
	if (GetAnimationFrame() == ATTACKMOUTH_COL_START) AttackStart();
	else if (GetAnimationFrame() == ATTACKMOUTH_COL_END) AttackEnd();
}

// ��ъ|����U������
void CDragon::Update_AttackHand()
{
	// �U���i�K�ɉ����ď��������s
	switch (mAttackStep)
	{
	case 0:// ��ъ|����U���n��
		// ��ъ|����U��
		if (ATTACKHAND_START_FRAME <= GetAnimationFrame())
		{
			// �h���S���������Ă�������ֈړ�
			mMoveSpeed += VectorZ() * ATTACKHAND_MOVESPEED;
		}
		// �W�����v����
		if (JUMP_START_FRAME <= GetAnimationFrame())
		{
			mMoveSpeed += CVector(0.0f, 3.6f, 0.0f);
			mIsGrounded = false;
			mAttackStep++;
		}
		break;
	case 1:// ��ъ|����U��
		if (ATTACKHAND_START_FRAME <= GetAnimationFrame())
		{
			// �h���S���������Ă�������ֈړ�
			mMoveSpeed += VectorZ() * ATTACKHAND_MOVESPEED;
		}
		// �����̒���
		if (FALLING_FRAME1_START <= GetAnimationFrame() &&
			GetAnimationFrame() <= FALLING_FRAME1_END)
		{
			mMoveSpeed += CVector(0.0f, -0.36f, 0.0f);
		}
		// ��ъ|����U���̈ړ����I��������
		if (ATTACKHAND_END_FRAME <= GetAnimationFrame())
		{
			mAttackStep++;
		}
		break;
	case 2:// �o�b�N�X�e�b�v
		// �o�b�N�X�e�b�v�ړ�����
		if (BACKSTEP_START_FRAME <= GetAnimationFrame() &&
			GetAnimationFrame() <= BACKSTEP_END_FRAME)
		{
			// �h���S���������Ă�������̋t�����ֈړ�
			mMoveSpeed += -VectorZ() * BACKSTEP_MOVESPEED;
		}
		// �����̒���
		if (FALLING_FRAME2_START <= GetAnimationFrame() &&
			GetAnimationFrame() <= FALLING_FRAME2_END)
		{
			mMoveSpeed += CVector(0.0f, -0.33f, 0.0f);
		}
		// �o�b�N�X�e�b�v(�W�����v)
		if (BACKJUMP_START_FRAME <= GetAnimationFrame())
		{
			mMoveSpeed += CVector(0.0f, 2.5f, 0.0f);
			mIsGrounded = false;
			mAttackStep++;
		}
		break;
	case 3:// �o�b�N�X�e�b�v
		// �o�b�N�X�e�b�v�ړ�����
		if (BACKSTEP_START_FRAME <= GetAnimationFrame() &&
			GetAnimationFrame() <= BACKSTEP_END_FRAME)
		{
			// �h���S���������Ă�������̋t�����ֈړ�
			mMoveSpeed += -VectorZ() * BACKSTEP_MOVESPEED;
		}
		// �����̒���
		if (FALLING_FRAME2_START <= GetAnimationFrame() &&
			GetAnimationFrame() <= FALLING_FRAME2_END)
		{
			mMoveSpeed += CVector(0.0f, -0.4f, 0.0f);
		}
		break;
	}

	//�n�ʂɑ������Ȃ��s����������邽��
	//�R���C�_�[�̈ʒu�𒲐��őΉ�
	if (18.0f <= GetAnimationFrame() && GetAnimationFrame() <= 25.0f)
	{
		mpColliderLine->Position(mpColliderLine->Position() + CVector(0.0f, 0.4f, 0.0f));
	}
	if (85.0f <= GetAnimationFrame())
	{
		mpColliderLine->Position(CVector::zero);
	}
}

// �n��u���X�U������
void CDragon::Update_AttackFlame()
{
	if (FLAME_START_FRAME <= GetAnimationFrame() &&
		GetAnimationFrame() <= FLAME_END_FRAME)
	{
		if (!mpFlamethrower->IsThrowing())
		{
			mpFlamethrower->Start();
		}
	}
	else if (mpFlamethrower->IsThrowing())
	{
		mpFlamethrower->Stop();
	}
}

// �o�b�N�X�e�b�v
void CDragon::Update_BackStep()
{
	switch (mAttackStep)
	{
	case 0:
		// �W�����v����
		if (12.0f <= GetAnimationFrame())
		{
			mMoveSpeed += CVector(0.0f, 2.5f, 0.0f);
			mIsGrounded = false;
			mAttackStep++;
		}
		// �ړ�����
		if (12.0f <= GetAnimationFrame() &&
			GetAnimationFrame() <= 21.0f)
		{
			mMoveSpeed += -VectorZ() * 4.5;
		}
		break;
	case 1:
		// �ړ�����
		if (12.0f <= GetAnimationFrame() &&
			GetAnimationFrame() <= 32.0f)
		{
			mMoveSpeed += -VectorZ() * 4.5;
		}
		// �����̒���
		if (22.0f <= GetAnimationFrame() &&
			GetAnimationFrame() <= 32.0f)
		{
			mMoveSpeed += CVector(0.0f, -0.3f, 0.0f);
		}
		break;
	}

	// �R���C�_�[����
	if (GetAnimationFrame() >= 0.0f)
	{
		mpColliderLine->Position(0.0f, 13.0f, 0.0f);
	}
	if (GetAnimationFrame() >= 20.0f)
	{
		mpColliderLine->Position(CVector::zero);
	}
	if (IsAnimationFinished()) mAttackStep = 0;
}

// �c���HP�̊������擾
int CDragon::GetHPPercent() const
{
	float hpper = Math::Clamp01((float)mStatus.hp / mMaxStatus.hp);
	int persent = hpper * 100;
	return persent;
}

// �퓬�A�C�h�����̃����_���l����
int CDragon::BattleIdleRand() const
{
	// �c��HP�̊������擾
	int persent = GetHPPercent();
	// 0�`100�̗����𐶐�
	int rand = Math::Rand(0.0f, 100.0f);

	// �{���Ԃ��ǂ���
	if (!mIsAngry)
	{
		// �c���HP�̊����ɂ���Đ������闐���𒲐�
		// 20%�ȉ�
		if (persent <= 20)
		{
			if (rand <= 40) return 1;
			else if (rand <= 85) return 2;
			else return 3;
		}
		// 50%�ȉ�
		else if (persent <= 50)
		{
			if (rand <= 30) return 1;
			else if (rand <= 80) return 2;
			else return 3;
		}
		// 75%�ȉ�
		else if (persent <= 75)
		{
			if (rand <= 20) return 1;
			else if (rand <= 70) return 2;
			else return 3;
		}
		// 100%�ȉ�
		else
		{
			if (rand <= 30) return 1;
			else if (rand <= 80) return 2;
			else return 3;
		}
	}
	else // �{����
	{
		// �c���HP�̊����ɂ���Đ������闐���𒲐�
		// 20%�ȉ�
		if (persent <= 20)
		{
			if (rand <= 35) return 1;
			else if (rand <= 80) return 2;
			else return 3;
		}
		// 50%�ȉ�
		else if (persent <= 50)
		{
			if (rand <= 25) return 1;
			else if (rand <= 75) return 2;
			else return 3;
		}
		// 100%�ȉ�
		else
		{
			if (rand <= 10) return 1;
			else if (rand <= 70) return 2;
			else return 3;
		}
	}
	return 0;
}