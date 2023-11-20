#include "CPlayer.h"

void CPlayer::Update_AttackAngleVec(CVector *direction)
{
	mMoveSpeed.X(0.0f);
	mMoveSpeed.Z(0.0f);

	// �L�[�̓��̓x�N�g�����擾
	CVector input = CVector::zero;
	bool isinput = false;	// �L�[�����͂���Ă��邩����
	if (CInput::Key('W'))		input.Z(-1.0f);
	else if (CInput::Key('S'))	input.Z(1.0f);
	if (CInput::Key('A'))		input.X(-1.0f);
	else if (CInput::Key('D'))	input.X(1.0f);

	// ���ۑ��̓��̓x�N�g���������l�̏ꍇ
	if (mInput_save == CVector::zero)
	{
		// ���̓x�N�g���f�[�^���ꎞ�I�ɕۑ�
		mInput_save = input;
		isinput = true;
	}
	// �U���A�j���[�V�������I��������
	// �ꎞ�I�ɕۑ��������̓x�N�g��������������
	if (IsAnimationFinished())
	{
		mInput_save = CVector::zero;
		isinput = false;
	}

	// �v���C���[�̉�]�p�x�ɂ���ăx�N�g�������
	CVector vec;
	CVector angle = EulerAngles();
	// 316 �` 45�x�̊�
	if (0 <= angle.Y() && angle.Y() <= 45 ||
		316 <= angle.Y() && angle.Y() <= 365)		vec.Z(1.0f);
	// 46 �` 135�x�̊�
	else if (46 <= angle.Y() && angle.Y() <= 135)	vec.X(-1.0f);
	// 136 �` 225�x�̊�
	else if (136 <= angle.Y() && angle.Y() <= 225)	vec.Z(-1.0f);
	// 226 �` 315�x�̊�
	else if (226 <= angle.Y() && angle.Y() <= 315)	vec.X(1.0f);

	// �J�����̌����ɍ��킹���ړ��x�N�g���ɕϊ�
	// �ړ��L�[���͂����邩�ɂ���Ĉړ��x�N�g����ύX
	CVector anglevec = CVector::zero;
	if (isinput) {
		anglevec = CCamera::MainCamera()->Rotation() * vec;
	}
	else {
		anglevec = CCamera::MainCamera()->Rotation() * mInput_save;
	}
	anglevec.Y(0.0f);
	anglevec.Normalize();

	*direction = anglevec;
}

// �ʏ�U��1-1����
void CPlayer::Update_NormalAttack1()
{
	CVector anglevec;
	Update_AttackAngleVec(&anglevec);

	switch (mAttackStep)
	{
	case 0:
		if (IsAnimationFinished())
		{
			mAttackStep++;
			ChangeAnimation(EAnimType::eNormalWait1_1);
		}
		// �U���ɍ��킹�ăv���C���[���ړ�
		if (NORMALATTACK1_1_START_FRAME <= GetAnimationFrame() &&
			GetAnimationFrame() <= NORMALATTACK1_1_END_FRAME)
		{
			mMoveSpeed += anglevec * NORMALATTACK1_1_MOVESPEED;
		}
		break;
	case 1:
		if (CInput::PushKey(VK_LBUTTON))
		{
			// �ʏ�U��1-2�֐؂�ւ�
			mAttackStep = 0;
			ChangeAnimation(EAnimType::eNormalAttack1_2);
		}
		else if (CInput::PushKey(VK_MBUTTON))
		{
			// �W�����v�U���֐؂�ւ�
			mAttackStep = 0;
			ChangeAnimation(EAnimType::eAttack_Up);
		}
		if (CInput::Key('C') && CInput::Key(VK_LBUTTON))
		{
			// �㓬�Z�֐؂�ւ�
			mAttackStep = 0;
			mState = EState::eSpecalMove;
			ChangeAnimation(EAnimType::eCounter_Start);
		}
		if (IsAnimationFinished())
		{
			mAttackStep = ATTACKSTEP_END;
			ChangeAnimation(EAnimType::eNormalEnd1_1);
		}
		Update_AttackWait();
		break;
	}
}

// �ʏ�U��1-2����
void CPlayer::Update_NormalAttack2()
{
	CVector anglevec;
	Update_AttackAngleVec(&anglevec);

	switch (mAttackStep)
	{
	case 0:
		if (IsAnimationFinished())
		{
			mAttackStep++;
			ChangeAnimation(EAnimType::eNormalWait1_2);
		}
		// �U���ɍ��킹�ăv���C���[���ړ�
		if (NORMALATTACK1_2_START_FRAME <= GetAnimationFrame() &&
			GetAnimationFrame() <= NORMALATTACK1_2_END_FRAME)
		{
			mMoveSpeed += anglevec * NORMALATTACK1_2_MOVESPEED;
		}
		break;
	case 1:
		if (CInput::PushKey(VK_LBUTTON))
		{
			// �ʏ�U��1-3�֐؂�ւ�
			mAttackStep = 0;
			ChangeAnimation(EAnimType::eNormalAttack1_3);
		}
		else if (CInput::PushKey(VK_MBUTTON))
		{
			// �W�����v�U���֐؂�ւ�
			mAttackStep = 0;
			ChangeAnimation(EAnimType::eAttack_Up);
		}
		if (CInput::Key('C') && CInput::Key(VK_LBUTTON))
		{
			// �㓬�Z�֐؂�ւ�
			mAttackStep = 0;
			mState = EState::eSpecalMove;
			ChangeAnimation(EAnimType::eCounter_Start);
		}
		if (IsAnimationFinished())
		{
			mAttackStep = ATTACKSTEP_END;
			ChangeAnimation(EAnimType::eNormalEnd1_2);
		}
		Update_AttackWait();
		break;
	}
}

// �ʏ�U��1-3����
void CPlayer::Update_NormalAttack3()
{
	CVector anglevec;
	Update_AttackAngleVec(&anglevec);
	switch (mAttackStep)
	{
	case 0:
		if (IsAnimationFinished())
		{
			mAttackStep++;
			ChangeAnimation(EAnimType::eNormalWait1_3);
		}
		// �U���ɍ��킹�ăv���C���[���ړ�
		if (NORMALATTACK1_3_START_FRAME <= GetAnimationFrame() &&
			GetAnimationFrame() <= NORMALATTACK1_3_END_FRAME)
		{
			mMoveSpeed += anglevec * NORMALATTACK1_3_MOVESPEED;
		}
		break;
	case 1:
		if (CInput::PushKey(VK_LBUTTON))
		{
			// �ʏ�U��1-1�֐؂�ւ�
			mAttackStep = 0;
			ChangeAnimation(EAnimType::eNormalAttack1_1);
		}
		else if (CInput::PushKey(VK_MBUTTON))
		{
			// �W�����v�U���֐؂�ւ�
			mAttackStep = 0;
			ChangeAnimation(EAnimType::eAttack_Up);
		}
		if (CInput::Key('C') && CInput::Key(VK_LBUTTON))
		{
			// �㓬�Z�֐؂�ւ�
			mAttackStep = 0;
			mState = EState::eSpecalMove;
			ChangeAnimation(EAnimType::eCounter_Start);
		}
		if (IsAnimationFinished())
		{
			mAttackStep = ATTACKSTEP_END;
			ChangeAnimation(EAnimType::eNormalEnd1_3);
		}
		Update_AttackWait();
		break;
	}
}

// �W�����v�U������
void CPlayer::Update_AttackUp()
{
	CVector anglevec;
	Update_AttackAngleVec(&anglevec);
	switch (mAttackStep)
	{
	case 0:
		if (IsAnimationFinished())
		{
			mAttackStep++;
			ChangeAnimation(EAnimType::eIdleAir_Combat);
			mMoveSpeed = CVector::zero;
		}
		// �U���ɍ��킹�ăv���C���[���ړ�
		if (ATTACK_UP_START_FRAME <= GetAnimationFrame() &&
			GetAnimationFrame() <= ATTACK_UP_END_FRAME)
		{
			mMoveSpeed += anglevec * ATTACK_UP_MOVE_SPEED;
		}
		// ���[�V�����ɍ��킹�ăW�����v���s��
		if (ATTACK_UP_JUMPSTART_FRAME == GetAnimationFrame())
		{
			mMoveSpeed += CVector(0.0f, ATTACK_UP_JUMP_SPEED, 0.0f);
			mIsGrounded = false;
		}
		break;
	case 1:
		if (CInput::PushKey(VK_LBUTTON))
		{
			// �󒆍U��1-1�֐؂�ւ�
			mAttackStep = 0;
			ChangeAnimation(EAnimType::eAirAttack1_1);
			mIsAirAttack = true;
		}
		Update_AttackWait();
		break;
	}
}

// �󒆍U��1-1����
void CPlayer::Update_AirAttack1()
{
	CVector anglevec;
	Update_AttackAngleVec(&anglevec);
	switch (mAttackStep)
	{
	case 0:
		if (IsAnimationFinished())
		{
			mAttackStep++;
			ChangeAnimation(EAnimType::eAirAttackWait1_1);
		}
		// �U�����͗������Ȃ�
		mMoveSpeed += -mMoveSpeed;
		break;
	case 1:
		if (CInput::PushKey(VK_LBUTTON))
		{
			// �󒆍U��1-2�֐؂�ւ�
			mAttackStep = 0;
			ChangeAnimation(EAnimType::eAirAttack1_2);
		}
		if (IsAnimationFinished())
		{
			mAttackStep = ATTACKSTEP_END;
			ChangeAnimation(EAnimType::eAirAttackEnd1_1);
		}
		// �U�����͗������Ȃ�
		mMoveSpeed += CVector(0.0f, GRAVITY, 0.0f);
		break;
	}
}

// �󒆍U��1-2����
void CPlayer::Update_AirAttack2()
{
	CVector anglevec;
	Update_AttackAngleVec(&anglevec);
	switch (mAttackStep)
	{
	case 0:
		if (IsAnimationFinished())
		{
			mAttackStep++;
			ChangeAnimation(EAnimType::eAirAttackWait1_2);
		}
		// �U�����͗������Ȃ�
		mMoveSpeed += -mMoveSpeed;
		break;
	case 1:
		if (CInput::PushKey(VK_LBUTTON))
		{
			// �󒆍U��1-3�֐؂�ւ�
			mAttackStep = 0;
			ChangeAnimation(EAnimType::eAirAttack1_3);
		}
		if (IsAnimationFinished())
		{
			mAttackStep = ATTACKSTEP_END;
			ChangeAnimation(EAnimType::eAirAttackEnd1_2);
		}
		// �U�����͗������Ȃ�
		mMoveSpeed += CVector(0.0f, GRAVITY, 0.0f);
		break;
	}
}

// �󒆍U��1-3����
void CPlayer::Update_AirAttack3()
{
	CVector anglevec;
	Update_AttackAngleVec(&anglevec);
	switch (mAttackStep)
	{
	case 0:
		if (IsAnimationFinished())
		{
			mAttackStep++;
			ChangeAnimation(EAnimType::eAirAttackWait1_3);
		}
		// �U�����͗������Ȃ�
		mMoveSpeed += -mMoveSpeed;
		break;
	case 1:
		if (CInput::PushKey(VK_LBUTTON))
		{
			// �󒆍U��1-4�ɐ؂�ւ�
			mAttackStep = 0;
			ChangeAnimation(EAnimType::eAirAttack1_4);
		}
		if (IsAnimationFinished())
		{
			mAttackStep = ATTACKSTEP_END;
			ChangeAnimation(EAnimType::eAirAttackEnd1_3);
		}
		mMoveSpeed += -mMoveSpeed;
		break;
	}
}

// �󒆍U��1-4����
void CPlayer::Update_AirAttack4()
{
	CVector anglevec;
	Update_AttackAngleVec(&anglevec);
	if (IsAnimationFinished())
	{
		mAttackStep = ATTACKSTEP_END;
		ChangeAnimation(EAnimType::eAirAttackEnd1_4);
	}
	// �U�����͗������Ȃ�
	mMoveSpeed += -mMoveSpeed;
}

// �U���҂�
// ���ʂ̍U���҂����̏���
void CPlayer::Update_AttackWait()
{
	// �U���҂����[�V�����̊Ԃɉ������͂��Ȃ���΁A
	// �A�C�h����ԂɈڍs����
	if (IsAnimationFinished())
	{
		// �ҋ@��Ԃֈڍs
		mState = EState::eIdle;
		mAttackStep = 0;
	}

	if (mIsGrounded)
	{
		// �U���҂����[�V�������Ɉړ��{����L�[�̓��͂������
		// �����Ԃֈڍs����
		if (CInput::Key('W') || CInput::Key('A') ||
			CInput::Key('S') || CInput::Key('D'))
		{
			if (CInput::PushKey(VK_SPACE))
			{
				mAttackStep = 0;
				mState = EState::eAvoidance;
				ChangeAnimation(EAnimType::eRollStart_Combat);
			}
		}
	}
}

// �U���I��
void CPlayer::Update_AttackEnd()
{
	if (mIsGrounded)
	{
		// �ړ��L�[�����͂��ꂽ��ړ���Ԃֈڍs����
		if (CInput::Key('W') || CInput::Key('A') ||
			CInput::Key('S') || CInput::Key('D'))
		{
			mAttackStep = 0;
			mState = EState::eMove;
			ChangeAnimation(EAnimType::eRunStart_Combat);
			// ��𓮍�ւ̐؂�ւ�
			if (CInput::PushKey(VK_SPACE))
			{
				mState = EState::eAvoidance;
				ChangeAnimation(EAnimType::eRollStart_Combat);
			}
		}
	}
	// �U���I�����[�V�������I��������ҋ@��Ԃֈڍs����
	if (IsAnimationFinished())
	{
		mAttackStep = 0;
		mState = EState::eIdle;
	}
}

void CPlayer::Update_Attack()
{
	// �A�j���[�V�����ɂ���ď�����ύX
	switch (AnimationIndex())
	{
	case (int)EAnimType::eNormalAttack1_1:// �ʏ�U��1-1����
		Update_NormalAttack1();
		break;
	case (int)EAnimType::eNormalWait1_1:
		Update_NormalAttack1();
		break;
	case (int)EAnimType::eNormalAttack1_2:// �ʏ�U��1-2����
		Update_NormalAttack2();
		break;
	case (int)EAnimType::eNormalWait1_2:
		Update_NormalAttack2();
		break;
	case (int)EAnimType::eNormalAttack1_3:// �ʏ�U��1-3����
		Update_NormalAttack3();
		break;
	case (int)EAnimType::eNormalWait1_3:
		Update_NormalAttack3();
		break;
	case (int)EAnimType::eAttack_Up:// �W�����v�U������
		Update_AttackUp();
		break;
	case (int)EAnimType::eIdleAir_Combat:
		Update_AttackUp();
		break;
	case (int)EAnimType::eAirAttack1_1:// �󒆍U��1-1����
		Update_AirAttack1();
		break;
	case (int)EAnimType::eAirAttackWait1_1:
		Update_AirAttack1();
		break;
	case (int)EAnimType::eAirAttack1_2:// �󒆍U��1-2����
		Update_AirAttack2();
		break;
	case (int)EAnimType::eAirAttackWait1_2:
		Update_AirAttack2();
		break;
	case (int)EAnimType::eAirAttack1_3:// �󒆍U��1-3����
		Update_AirAttack3();
		break;
	case (int)EAnimType::eAirAttackWait1_3:
		Update_AirAttack3();
		break;
	case (int)EAnimType::eAirAttack1_4:// �󒆍U��1-4
		Update_AirAttack4();
		break;
	}

	// �U���������I��
	if (mAttackStep == ATTACKSTEP_END)
	{
		// �U���I�����������s
		Update_AttackEnd();
	}
}