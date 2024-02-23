#include "CPlayer.h"

// ���Z�ɕK�v�ȏ���ݒ�
void CPlayer::SetSpecalMove(EAnimType type, bool cutin)
{
	mAttackStep = 0;
	mState = EState::eSpecalMove;
	ChangeAnimation(type);

	// �J�b�g�C�����K�v�ȏꍇ
	// �J�b�g�C���J�����̐ݒ���s��
	if (cutin)
	{
		mpCutIn_PowerAttack->Setup(this);
		mpCutIn_PowerAttack->Start();
	}
}

//�@���Z����
void CPlayer::Update_SpecialMove()
{
	// �㓬�Z�������Z������
	// false:�㓬�Z�@true:�����Z
	bool isspmove = nullptr;

	// �A�j���[�V�����ɂ���ē��Z�̎㓬�Z�������Z�����肷��
	switch (AnimationIndex())
	{
	// �㓬�Z
	case (int)EAnimType::eCounter_Start:	isspmove = false; break;
	case (int)EAnimType::eCounter_Loop:		isspmove = false; break;
	case (int)EAnimType::eCounter_End:		isspmove = false; break;
	// �����Z
	case (int)EAnimType::ePowerAttack_Start:isspmove = true;  break;
	case(int)EAnimType::ePowerAttack_Loop:	isspmove = true;  break;
	case(int)EAnimType::ePowerAttack:		isspmove = true;  break;
	}

	// ���肵�����Z�̏��������s
	if (isspmove) {
		// �����Z
		Update_Strong_SpecalMove(); 
	}
	else {
		// �㓬�Z
		Update_Weak_SpecalMove();
	}

	// �U���������I��
	if (mAttackStep == ATTACKSTEP_END)
	{
		Update_AttackEnd();
	}
}


// �㓬�Z����
void CPlayer::Update_Weak_SpecalMove()
{
	mMoveSpeed.X(0.0f);
	mMoveSpeed.Z(0.0f);

	switch (mAttackStep)
	{
	case 0:// �J�E���^�[�\���n��
		if (mIsCounter)
		{
			// �J�E���^�[�U�������s
			mAttackStep = 2;
			ChangeAnimation(EAnimType::eParryAttack);
		}
		if (IsAnimationFinished())
		{
			mAttackStep++;
			ChangeAnimation(EAnimType::eCounter_Loop);
		}
		break;
	case 1:// �J�E���^�[�\����
		if (mIsCounter)
		{
			// �J�E���^�[�U�������s
			mAttackStep = 2;
			ChangeAnimation(EAnimType::eParryAttack);
		}
		if (IsAnimationFinished())
		{
			mAttackStep = ATTACKSTEP_END;
			ChangeAnimation(EAnimType::eCounter_End);
		}
		break;
	case 2:
		// ����ɍU���J�n���g����
		mpSword->AttackStart();
		mAttackStep++;
		break;
	case 3:// �J�E���^�[�U��
		// �J�E���^�[�U�����I��
		if (IsAnimationFinished())
		{
			mIsCounter = false;
			mAttackStep = ATTACKSTEP_END;
			ChangeAnimation(EAnimType::eParryAttack_End);
			// ����ɍU���I����`����
			mpSword->AttackEnd();
		}
		break;
	}
}

// �����Z����
void CPlayer::Update_Strong_SpecalMove()
{
	mMoveSpeed.X(0.0f);
	mMoveSpeed.Z(0.0f);

	switch (mAttackStep)
	{
	case 0:// ���U���\���J�n
		if (IsAnimationFinished())
		{
			mAttackStep++;
			ChangeAnimation(EAnimType::ePowerAttack_Loop);
		}
		break;
	case 1:// ���U���\����
		if (IsAnimationFinished())
		{
			mAttackStep++;
			ChangeAnimation(EAnimType::ePowerAttack);
			Strong_SpecalMove_Move();
		}
		break;
	case 2:// �����Z
		// �U���ɍ��킹�ăv���C���[���ړ�
		Strong_SpecalMove_Move();
		// �����Z�I��
		if (IsAnimationFinished())
		{
			// ����ɍU���I����`����
			mpSword->AttackEnd();
			mAttackStep = ATTACKSTEP_END;
			mSPAttackStep = 0;
		}
		break;
	}
}

// �����Z���̃v���C���[�̈ړ�����
void CPlayer::Strong_SpecalMove_Move()
{
	switch (mSPAttackStep)
	{
	case 0:// ��x�ڂ̈ړ�����
		// �n�߂�1�t���[���ڂɌ��ɍU���J�n��`����
		if (GetAnimationFrame() == POWERATTACK1_START) mpSword->AttackStart();

		if (POWERATTACK1_START <= GetAnimationFrame() &&
			GetAnimationFrame() <= POWERATTACK1_END)
		{
			mMoveSpeed += POWERATTACK1_VEC * POWERATTACK_MOVESPEED;
		}
		// �ړ����I�������玟�̃X�e�b�v�Ɉڍs
		else if (GetAnimationFrame() > POWERATTACK1_END)
		{
			// ���ɍU���I����`����
			mpSword->AttackEnd();
			mSPAttackStep++;
		}
		break;
	case 1:// ��x�ڂ̈ړ�����
		// �n�߂�1�t���[���ڂɌ��ɍU���J�n��`����
		if (GetAnimationFrame() == POWERATTACK2_START) mpSword->AttackStart();

		if (POWERATTACK2_START <= GetAnimationFrame() &&
			GetAnimationFrame() <= POWERATTACK2_END)
		{
			mMoveSpeed += POWERATTACK2_VEC * POWERATTACK_MOVESPEED;
		}
		// �ړ����I�������玟�̃X�e�b�v�Ɉڍs
		else if (GetAnimationFrame() > POWERATTACK2_END)
		{
			// ���ɍU���I����`����
			mpSword->AttackEnd();
			mSPAttackStep++;
		}
		break;
	case 2:// �O�x�ڂ̈ړ�����
		// �n�߂�1�t���[���ڂɌ��ɍU���J�n��`����
		if (GetAnimationFrame() == POWERATTACK3_START) mpSword->AttackStart();

		if (POWERATTACK3_START <= GetAnimationFrame() &&
			GetAnimationFrame() <= POWERATTACK3_END)
		{
			mMoveSpeed += POWERATTACK3_VEC * POWERATTACK_MOVESPEED;
		}
		// �ړ����I�������玟�̃X�e�b�v�Ɉڍs
		else if (GetAnimationFrame() > POWERATTACK3_END)
		{
			// ���ɍU���I����`����
			mpSword->AttackEnd();
			mSPAttackStep++;
		}
		break;
	case 3:// �l�x�ڂ̈ړ�����
		// �n�߂�1�t���[���ڂɌ��ɍU���J�n��`����
		if (GetAnimationFrame() == POWERATTACK4_START) mpSword->AttackStart();

		if (POWERATTACK4_START <= GetAnimationFrame() &&
			GetAnimationFrame() <= POWERATTACK4_END)
		{
			mMoveSpeed += POWERATTACK4_VEC * POWERATTACK_MOVESPEED;
		}
		// �ړ����I�������玟�̃X�e�b�v�Ɉڍs
		else if (GetAnimationFrame() > POWERATTACK4_END)
		{
			// ���ɍU���I����`����
			mpSword->AttackEnd();
			mSPAttackStep++;
		}
		break;
	case 4:// �ܓx�ڂ̈ړ�����
		// �n�߂�1�t���[���ڂɌ��ɍU���J�n��`����
		if (GetAnimationFrame() == POWERATTACK5_START) mpSword->AttackStart();

		if (POWERATTACK5_START <= GetAnimationFrame() &&
			GetAnimationFrame() <= POWERATTACK5_END)
		{
			mMoveSpeed += POWERATTACK5_VEC * POWERATTACK_MOVESPEED;
		}
		// �ړ����I�������玟�̃X�e�b�v�Ɉڍs
		else if (GetAnimationFrame() > POWERATTACK5_END)
		{
			// ���ɍU���I����`����
			mpSword->AttackEnd();
			mSPAttackStep++;
		}
		break;
	case 5:// �Z�x�ڂ̈ړ�����
		// �n�߂�1�t���[���ڂɌ��ɍU���J�n��`����
		if (GetAnimationFrame() == POWERATTACK6_START) mpSword->AttackStart();

		if (POWERATTACK6_START <= GetAnimationFrame() &&
			GetAnimationFrame() <= POWERATTACK6_END)
		{
			mMoveSpeed += POWERATTACK6_VEC * POWERATTACK_MOVESPEED;
		}
		// �ړ����I�������玟�̃X�e�b�v�Ɉڍs
		else if (GetAnimationFrame() >= POWERATTACK6_END)
		{
			// ���ɍU���I����`����
			mpSword->AttackEnd();
			mSPAttackStep++;
		}
		break;
	case 6:// ���x�ڂ̈ړ�����
		// �n�߂�1�t���[���ڂɌ��ɍU���J�n��`����
		if (GetAnimationFrame() == POWERATTACK7_START) mpSword->AttackStart();

		if (POWERATTACK7_START <= GetAnimationFrame() &&
			GetAnimationFrame() <= POWERATTACK7_END)
		{
			mMoveSpeed -= POWERATTACL7_VEC;
		}
		if (GetAnimationFrame() == POWERATTACK7_END) mpSword->AttackEnd();
		break;
	}
}