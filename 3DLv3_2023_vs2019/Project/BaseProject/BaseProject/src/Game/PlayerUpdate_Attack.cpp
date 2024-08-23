#include "CPlayer.h"

// �v���C���[�̌����Ă�������̃x�N�g�����擾
void CPlayer::Update_AttackAngleVec(CVector *direction)
{
	mMoveSpeed.X(0.0f);
	mMoveSpeed.Z(0.0f);

	// �L�[�̓��̓x�N�g�����擾
	CVector input = CVector::zero;
	if (CInput::Key('W'))		input.Z(-1.0f);
	else if (CInput::Key('S'))	input.Z(1.0f);
	if (CInput::Key('A'))		input.X(-1.0f);
	else if (CInput::Key('D'))	input.X(1.0f);

	// ���ۑ��̓��̓x�N�g���������l�̏ꍇ
	if (mIsUpdateInput && input.LengthSqr() > 0.0f) 
	{
		// ���̓x�N�g���f�[�^���ꎞ�I�ɕۑ�
		mInput_save = mCamForward * input.Z() + mCamSide * input.X();
		mIsUpdateInput = false;
	}
	// �U���A�j���[�V�������I��������
	// �ꎞ�I�ɕۑ��������̓x�N�g��������������
	if (IsAnimationFinished())
	{
		mIsUpdateInput = true;
	}

	// �J�����̌����ɍ��킹���ړ��x�N�g���ɕϊ�
	// �ړ��L�[���͂����邩�ɂ���Ĉړ��x�N�g����ύX
	CVector anglevec = CVector::zero;
	if (mIsUpdateInput || mInput_save.LengthSqr() == 0.0f) {
		anglevec = VectorZ();
	}
	else {
		anglevec = mInput_save;
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
	CVector pos = Position();
	switch (mAttackStep)
	{
	case 0:
	{
		// ���ɍU���J�n��`����
		mpSword->AttackStart();
		mAttackStep++;
		mpNormalAttackSE1->Play(0.2f);
	}
	case 1:
		if (IsAnimationFinished())
		{
			mAttackStep++;
			ChangeAnimation(EAnimType::eNormalWait1_1);
			// ���ɍU���I����`����
			mpSword->AttackEnd();
		}
		// �U���ɍ��킹�ăv���C���[���ړ�
		if (NORMALATTACK1_1_START_FRAME <= GetAnimationFrame() &&
			GetAnimationFrame() <= NORMALATTACK1_1_END_FRAME)
		{
			mMoveSpeed += anglevec * NORMALATTACK1_1_MOVESPEED;
		}
		break;
	case 2:
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
		if (IsAnimationFinished())
		{
			mAttackStep = ATTACKSTEP_END;
			ChangeAnimation(EAnimType::eNormalEnd1_1);
		}

		// ���Z�ւ̐؂�ւ����͂��L��Γ��Z�ֈڍs
		ChangeSpMove();
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
	{
		// ���ɍU���J�n��`����
		mpSword->AttackStart();
		mAttackStep++;
		mpNormalAttackSE2->Play(0.2f);
	}
		break;
	case 1:
		if (IsAnimationFinished())
		{
			mAttackStep++;
			ChangeAnimation(EAnimType::eNormalWait1_2);
			// ���ɍU���I����`����
			mpSword->AttackEnd();
		}
		// �U���ɍ��킹�ăv���C���[���ړ�
		if (NORMALATTACK1_2_START_FRAME <= GetAnimationFrame() &&
			GetAnimationFrame() <= NORMALATTACK1_2_END_FRAME)
		{
			mMoveSpeed += anglevec * NORMALATTACK1_2_MOVESPEED;
		}
		break;
	case 2:
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
		if (IsAnimationFinished())
		{
			mAttackStep = ATTACKSTEP_END;
			ChangeAnimation(EAnimType::eNormalEnd1_2);
		}
		// ���Z�ւ̐؂�ւ����͂��L��Γ��Z�ֈڍs
		ChangeSpMove();
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
	{
		// ���ɍU���J�n��`����
		mpSword->AttackStart();
		mAttackStep++;
		mpNormalAttackSE1->Play(0.2f);
	}
		break;
	case 1:
		// �U���ɍ��킹�ăv���C���[���ړ�
		if (NORMALATTACK1_3_START_FRAME <= GetAnimationFrame() &&
			GetAnimationFrame() <= NORMALATTACK1_3_END_FRAME)
		{
			mMoveSpeed += anglevec * NORMALATTACK1_3_MOVESPEED;
		}
		if (GetAnimationFrame() >= NORMALATTACK1_3_END_FRAME)
		{
			mAttackStep++;
			mpSword->AttackEnd();
		}
		break;
	case 2:
	{
		if (NORMALATTACK1_3_COLLIDER <= GetAnimationFrame())
		{
			mpSword->AttackStart();
			mAttackStep++;
			mpNormalAttackSE2->Play(0.2f);
		}
	}
		break;
	case 3:
		if (IsAnimationFinished())
		{
			mAttackStep++;
			ChangeAnimation(EAnimType::eNormalWait1_3);
			// ���ɍU���I����`����
			mpSword->AttackEnd();
		}
		break;
	case 4:
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
		if (IsAnimationFinished())
		{
			mAttackStep = ATTACKSTEP_END;
			ChangeAnimation(EAnimType::eNormalEnd1_3);
		}
		// ���Z�ւ̐؂�ւ����͂��L��Γ��Z�ֈڍs
		ChangeSpMove();
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
	{
		// ���ɍU���J�n��`����
		mpSword->AttackStart();
		mAttackStep++;
		CSound* jumpse = CResourceManager::Get<CSound>("JumpAttackSE");
		jumpse->Play(0.2f);
	}
		break;
	case 1:
		if (IsAnimationFinished())
		{
			mAttackStep++;
			ChangeAnimation(EAnimType::eIdleAir_Combat);
			mMoveSpeed = CVector::zero;
			// ���ɍU���I����`����
			mpSword->AttackEnd();
		}
		// �U���ɍ��킹�ăv���C���[���ړ�
		if (ATTACK_UP_START_FRAME <= GetAnimationFrame() &&
			GetAnimationFrame() <= ATTACK_UP_END_FRAME)
		{
			mMoveSpeed += anglevec * ATTACK_UP_MOVE_SPEED;
		}
		// ���[�V�����ɍ��킹�ăW�����v���s��
		if (mIsGrounded && 
			ATTACK_UP_JUMPSTART_FRAME <= GetAnimationFrame()){
			mMoveSpeed += CVector(0.0f, ATTACK_UP_JUMP_SPEED, 0.0f);
			mIsGrounded = false;
		}

		break;
	case 2:
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
		// ���ɍU���J�n��`����
		mpSword->AttackStart();
		mAttackStep++;
		mpNormalAttackSE1->Play(0.2f);
		break;
	case 1:
		if (IsAnimationFinished())
		{
			mAttackStep++;
			ChangeAnimation(EAnimType::eAirAttackWait1_1);
			// ���ɍU���I����`����
			mpSword->AttackEnd();
		}
		// �U�����͗������Ȃ�
		mMoveSpeed += -mMoveSpeed;
		break;
	case 2:
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
		// ���ɍU���J�n��`����
		mpSword->AttackStart();
		mAttackStep++;
		mpNormalAttackSE1->Play(0.2f);
		break;
	case 1:
		if (IsAnimationFinished())
		{
			mAttackStep++;
			ChangeAnimation(EAnimType::eAirAttackWait1_2);
			// ���ɍU���I����`����
			mpSword->AttackEnd();
		}
		// �U�����͗������Ȃ�
		mMoveSpeed += -mMoveSpeed;
		break;
	case 2:
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
		// ���ɍU���J�n��`����
		mpSword->AttackStart();
		mAttackStep++;
		mpNormalAttackSE1->Play(0.2f);
		break;
	case 1:
		if (IsAnimationFinished())
		{
			mAttackStep++;
			ChangeAnimation(EAnimType::eAirAttackWait1_3);
			// ���ɍU���I����`����
			mpSword->AttackEnd();
		}
		// �U�����͗������Ȃ�
		mMoveSpeed += -mMoveSpeed;
		break;
	case 2:
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
	switch (mAttackStep)
	{
	case 0:
		// ���ɍU���J�n��`����
		mpSword->AttackStart();
		mAttackStep++;
		mpNormalAttackSE1->Play(0.2f);
		break;
	case 1:
		CVector anglevec;
		Update_AttackAngleVec(&anglevec);
		if (IsAnimationFinished())
		{
			mAttackStep = ATTACKSTEP_END;
			ChangeAnimation(EAnimType::eAirAttackEnd1_4);
			// ���ɍU���I����`����
			mpSword->AttackEnd();
		}
		// �U�����͗������Ȃ�
		mMoveSpeed += -mMoveSpeed;
		break;
	}
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
		ChangeState(EState::eIdle);
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
				if (mStatus.sp >= AVOID_SPCOST)
				{
					mAttackStep = 0;
					ChangeState(EState::eAvoidance);
					ChangeAnimation(EAnimType::eRollStart_Combat);
				}
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
			ChangeState(EState::eMove);
			ChangeState(EState::eMove);
			ChangeAnimation(EAnimType::eRunStart_Combat);
			// ��𓮍�ւ̐؂�ւ�
			if (CInput::PushKey(VK_SPACE))
			{
				if (mStatus.sp >= AVOID_SPCOST)
				{
					ChangeState(EState::eAvoidance);
					ChangeAnimation(EAnimType::eRollStart_Combat);
				}
			}
		}
	}
	// �U���I�����[�V�������I��������ҋ@��Ԃֈڍs����
	if (IsAnimationFinished())
	{
		ChangeState(EState::eIdle);
		mAttackStep = 0;
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

void CPlayer::ChangeSpMove()
{
	if (CInput::Key('C') && CInput::Key(VK_LBUTTON))
	{
		// ���C��100�ȏ゠���
		if (mStatus.touki >= 100)
		{
			// �㓬�Z�֐؂�ւ�
			SetSpecalMove(EAnimType::eCounter_Start, false);
			mStatus.touki -= 100;
		}
	}
	if (CInput::Key('C') && CInput::Key(VK_RBUTTON))
	{
		// ���C��200�ȏ゠���
		if (mStatus.touki >= 200)
		{
			// �����Z�֐؂�ւ�
			SetSpecalMove(EAnimType::ePowerAttack_Start, true);
			mStatus.touki -= 200;
		}
	}
}