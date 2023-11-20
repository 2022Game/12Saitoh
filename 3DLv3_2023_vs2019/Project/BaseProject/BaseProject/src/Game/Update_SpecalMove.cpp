#include "CPlayer.h"

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
	case (int)EAnimType::Num: isspmove = true; break;
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
	switch (mAttackStep)
	{
	case 0:// �J�E���^�[�\���n��
		if (mIsCounter)
		{
			// �J�E���^�[�U�������s
			mAttackStep = ATTACKSTEP_COUNTER;
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
			mAttackStep = ATTACKSTEP_COUNTER;
			ChangeAnimation(EAnimType::eParryAttack);
		}
		if (IsAnimationFinished())
		{
			mAttackStep = ATTACKSTEP_END;
			ChangeAnimation(EAnimType::eCounter_End);
		}
		break;
	case ATTACKSTEP_COUNTER:// �J�E���^�[�U��
		// �J�E���^�[�U�����I��
		if (IsAnimationFinished())
		{
			mIsCounter = false;
			mAttackStep = ATTACKSTEP_END;
			ChangeAnimation(EAnimType::eParryAttack_End);
		}
		break;
	}
}

// �����Z����
void CPlayer::Update_Strong_SpecalMove()
{

}
