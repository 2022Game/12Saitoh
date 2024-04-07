#include "CDragon.h"
#include "Maths.h"
#include "Time.h"
#include "CColliderLine.h"
#include "CPlayer.h"
#include "CFlamethrower.h"

// �퓬���̍X�V����
void CDragon::UpdateBattle()
{
	// �{���Ԃ��ǂ���
	if (!mIsAngry)// �ʏ���
	{
		// �U�����I�������A�C�h���ɖ߂�
		if (IsAnimationFinished())
		{
			ChangeAnimation(EDragonAnimType::eIdle1);
			mIdleFlag = true;
			// �u���X�U�������Ă�����A�u���X���~�߂�
			if (mpFlamethrower->IsThrowing())mpFlamethrower->Stop();
		}

		// �A�C�h�����Ȃ�o�ߎ��Ԃ����Z����
		if (mIdleFlag) mElapsedTime += Time::DeltaTime();

		// 5�b���ƂɍU�����s��
		if (mElapsedTime >= 5.0f)
		{
			// �o�ߎ��Ԃ�������
			mElapsedTime = 0.0f;
			mIdleFlag = false;
			mIsAttack = false;

			// �v���C���[�Ƃ̋������擾
			PlayerFromDistance();
		}

		// �U�����łȂ����
		if (!mIsAttack)
		{
			// �����ɉ����ď������s��
			switch (mDistanceType)
			{
			case EDistanceType::eNear:// �ߋ���
			{
				CVector playerPos = CPlayer::Instance()->Position();
				CVector enemyPos = Position();
				// ���g����v���C���[�܂ł̃x�N�g���̒������擾
				float distance = (playerPos - enemyPos).Length();

				// ���݂��U���͈͓̔��������ꍇ
				if (distance <= 120.0f)
				{
					// ���݂��U�����s��
					ChangeAnimation(EDragonAnimType::eAttackMouth);
					mIsAttack = true;
				}
				else
				{
					// ���g����v���C���[�܂ł̃x�N�g�����擾
					CVector EP = (playerPos - enemyPos).Normalized();
					// ���݂��U���͈̔͂܂ňړ�����
					mMoveSpeed += EP * 0.01f;
					ChangeAnimation(EDragonAnimType::eWalk);
				}
				break;
			}
			case EDistanceType::eMedium:// ������
				// �����_���l�𐶐����āA�s���U����I��
				// ��ъ|����U�������߂ɂȂ�悤����
				switch (Math::Rand(0, 1))
				{
				case 0:
				case 1:// �u���X�U��
					ChangeAnimation(EDragonAnimType::eAttackFlame);
					mIsAttack = true;
					break;
				default:// ��ъ|����U��
					ChangeAnimation(EDragonAnimType::eAttackHand);
					mIsAttack = true;
					break;
				}
				break;

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
					CVector playerPos = CPlayer::Instance()->Position();
					CVector enemyPos = Position();
					// ���g����v���C���[�܂ł̃x�N�g���̒������擾
					float distance = (playerPos - enemyPos).Length();

					// �ړ�
					if (distance >= 230.0f)
					{
						// ���g����v���C���[�܂ł̃x�N�g�����擾
						CVector EP = (playerPos - enemyPos).Normalized();
						// ���݂��U���͈̔͂܂ňړ�����
						mMoveSpeed += EP * 0.1f;
						ChangeAnimation(EDragonAnimType::eWalk);
					}
					// ���͈̔͂܂ňړ�������
					else
					{
						mRandSave = 0;
						// �����_���l�𐶐����āA�U�������߂�
						int rand = Math::Rand(0, 1);
						// �ߋ���
						if (rand == 0) mDistanceType = EDistanceType::eNear;
						// ������
						else mDistanceType = EDistanceType::eMedium;
					}
				}
				break;
				case 3:
					break;
				}
				break;
			}
			}
		}
	}
	else // �{����
	{
		// ��U�A�C�h����Ԃɂ���
		if (IsAnimationFinished())
		{
			ChangeAnimation(EDragonAnimType::eIdle1);
		}

		// �{���ԂɂȂ��Ă���̎��Ԃ��擾
		mAngryElapsedTime += Time::DeltaTime();

		// �{���ԂɂȂ��Ă���1���o�߂œ{���Ԃ�����
		if (mAngryElapsedTime >= 60.0f)
		{
			mIsAngry = false;
			mAngryElapsedTime = 0.0f;
		}
	}

	// �U�������̍X�V
	UpdateAttack();
}

// �U������
void CDragon::UpdateAttack()
{
	// �e�A�j���[�V�������Ƃɏ������s��
	switch (AnimationIndex())
	{
	case (int)EDragonAnimType::eScream:// ���K
		// ���K�p�̃R���C�_�̐؂�ւ��Ȃǂ��s���\��
		break;

	case (int)EDragonAnimType::eAttackMouth:// ���݂��U��
		mMoveSpeed.X(0.0f);
		mMoveSpeed.Z(0.0f);
		if (GetAnimationFrame() == ATTACKMOUTH_COL_START) AttackStart();
		else if (GetAnimationFrame() == ATTACKMOUTH_COL_END) AttackEnd();
		break;

	case (int)EDragonAnimType::eAttackHand:// ��ъ|����U��
		mMoveSpeed.X(0.0f);
		mMoveSpeed.Z(0.0f);
		// �ړ�����
		// ��ъ|����U��
		if (ATTACKHAND_START_FRAME <= GetAnimationFrame() &&
			GetAnimationFrame() <= ATTACKHAND_END_FRAME)
		{
			// �h���S���������Ă�������ֈړ�
			mMoveSpeed += VectorZ() * ATTACKHAND_MOVESPEED;
		}
		// ��ъ|����U����̃o�b�N�X�e�b�v
		else if (BACKSTEP_START_FRAME <= GetAnimationFrame() &&
			GetAnimationFrame() <= BACKSTEP_END_FRAME)
		{
			// �h���S���������Ă�������̋t�����ֈړ�
			mMoveSpeed += -VectorZ() * BACKSTEP_MOVESPEED;
		}

		// �W�����v����
		if (GetAnimationFrame() == JUMP_START_FRAME)
		{
			mMoveSpeed += CVector(0.0f, 4.0f, 0.0f);
			mIsGrounded = false;
		}
		// �����̒���
		else if (FALLING_FRAME1_START <= GetAnimationFrame() &&
			GetAnimationFrame() <= FALLING_FRAME1_END)
		{
			mMoveSpeed += CVector(0.0f, -0.4f, 0.0f);
		}
		// �o�b�N�X�e�b�v
		else if (GetAnimationFrame() == BACKJUMP_START_FRAME)
		{
			mMoveSpeed += CVector(0.0f, 2.0f, 0.0f);
			mIsGrounded = false;
		}
		// �����̒���
		else if (FALLING_FRAME2_START <= GetAnimationFrame() &&
			GetAnimationFrame() <= FALLING_FRAME2_END)
		{
			mMoveSpeed += CVector(0.0f, -0.33f, 0.0f);
		}

		// �n�ʂɑ������Ȃ��s����������邽��
		// �R���C�_�[�̈ʒu�𒲐��őΉ�
		if (36.0f <= GetAnimationFrame() && GetAnimationFrame() <= 60.0f)
		{
			mpColliderLine->Position(mpColliderLine->Position() + CVector(0.0f, 0.2f, 0.0f));
		}
		if (GetAnimationFrame() == 74.0f)
		{
			mpColliderLine->Position(0.0f, 4.0f, 0.0f);
		}
		else if (GetAnimationFrame() == 148.0f)
		{
			mpColliderLine->Position(CVector::zero);
		}
		break;

	case (int)EDragonAnimType::eAttackFlame:
		mMoveSpeed.X(0.0f);
		mMoveSpeed.Z(0.0f);

		if (FLAME_START_FRAME <= GetAnimationFrame()&&
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
		break;
	default:// �U�����ȊO�́A�v���C���[�̂�������։�](��)

		CVector playerPos = CPlayer::Instance()->Position();
		CVector enemyPos = Position();
		// ���g����v���C���[�܂ł̃x�N�g�����擾
		CVector EP = (playerPos - enemyPos).Normalized();
		
		// �v���C���[�̕����֌�����
		CVector current = VectorZ();
		CVector target = EP;
		target.Y(0.0f);

		CVector angle = CVector::Slerp(current, target, 0.02f);
		Rotation(CQuaternion::LookRotation(angle));
		break;
	}
}