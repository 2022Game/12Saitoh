#include "CDragon.h"
#include "Maths.h"
#include "Time.h"
#include "CColliderLine.h"
#include "CPlayer.h"

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
		}

		int index = AnimationIndex();
		if (index != (int)EDragonAnimType::eAttackHand &&
			index != (int)EDragonAnimType::eAttackFlame) 		
			mElapsedTime += Time::DeltaTime();

		// 5�b���ƂɍU�����s��
		if (mElapsedTime >= 5.0f)
		{
			// �o�ߎ��Ԃ�������
			mElapsedTime = 0.0f;

			// �ЂƂ܂������_���l�𐶐����āA�l�ɉ����čU�����s��
			int rand = Math::Rand(0, 1);
			switch (rand)
			{
			case 0:
				ChangeAnimation(EDragonAnimType::eAttackMouth);
				break;
			case 1:
				ChangeAnimation(EDragonAnimType::eAttackHand);
				break;
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
	mMoveSpeed.X(0.0f);
	mMoveSpeed.Z(0.0f);

	// �e�A�j���[�V�������Ƃɏ������s��
	switch (AnimationIndex())
	{
	case (int)EDragonAnimType::eScream:// ���K
		// ���K�p�̃R���C�_�̐؂�ւ��Ȃǂ��s���\��
		break;

	case (int)EDragonAnimType::eAttackMouth:// ���݂��U��
		if (GetAnimationFrame() == ATTACKMOUTH_COL_START) AttackStart();
		else if (GetAnimationFrame() == ATTACKMOUTH_COL_END) AttackEnd();
		break;

	case (int)EDragonAnimType::eAttackHand:// ��ъ|����U��
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