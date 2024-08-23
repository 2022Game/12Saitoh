#include "CPlayer.h"

//�ړ�
void CPlayer::Update_Move()
{
	mMoveSpeed.X(0.0f);
	mMoveSpeed.Z(0.0f);

	if (!mpRunSE->IsPlaying())
	{
		mpRunSE->PlayLoop(-1, true, 0.1f);
	}
	else
	{
		mpRunSE->SetVolume(0.1f);
	}
	//���[��Ԃ̔���
	switch (mIsDrawn)
	{
	case true:	// ����
		// �n�ʂɐڒn���Ă��邩����
		if (mIsGrounded)
		{
			// �ړ�����
			// �L�[�̓��̓x�N�g�����擾
			CVector input;
			if (CInput::Key('W'))		input.Z(-1.0f);
			else if (CInput::Key('S'))	input.Z(1.0f);
			if (CInput::Key('A'))		input.X(-1.0f);
			else if (CInput::Key('D'))	input.X(1.0f);

			// ���̓x�N�g���̒����œ��͂���Ă��邩����
			if (input.LengthSqr() > 0)
			{
				// �J�����̌����ɍ��킹���ړ��x�N�g���ɕϊ�
				CCamera* mainCamera = CCamera::MainCamera();
				CVector camForward = mainCamera->VectorZ();
				CVector camSide = CVector::Cross(CVector::up, camForward);
				CVector move = camForward * input.Z() + camSide * input.X();
				move.Y(0.0f);
				move.Normalize();

				mMoveSpeed += move * MOVE_SPEED;

				// �A�j���[�V�������I��
				if (IsAnimationFinished())
				{
					// ����A�j���[�V�����ɐ؂�ւ�
					ChangeAnimation(EAnimType::eRun_Combat);
				}
				// ��𓮍�ւ̐؂�ւ�
				if (CInput::PushKey(VK_SPACE))
				{
					if (mStatus.sp >= AVOID_SPCOST)
					{
						ChangeState(EState::eAvoidance);
						ChangeAnimation(EAnimType::eRollStart_Combat);
					}
				}
				// ���N���b�N�ōU����Ԃֈڍs
				if (CInput::PushKey(VK_LBUTTON))
				{
					ChangeState(EState::eAttack);
					ChangeAnimation(EAnimType::eNormalAttack1_1);
				}
				// �}�E�X�z�C�[���N���b�N�ŃW�����v�U���ֈڍs
				if (CInput::PushKey(VK_MBUTTON))
				{
					ChangeState(EState::eAttack);
					ChangeAnimation(EAnimType::eAttack_Up);
				}
				// E�L�[�Ŕ[��
				if (CInput::PushKey('E'))
				{
					ChangeState(EState::eMove);
					ChangeAnimation(EAnimType::eRun_Sheathed_Combat);
				}
				// ���Z�ւ̐؂�ւ����͂��L��Γ��Z�ֈڍs
				ChangeSpMove();
			}
			// �ړ��L�[��������Ă��Ȃ�
			else
			{
				// ����I���̃A�j���[�V�������Đ����A�A�C�h����Ԃɖ߂�
				ChangeAnimation(EAnimType::eRunEnd_Combat);
				ChangeState(EState::eIdle);
			}
		}
		else
		{
			ChangeAnimation(EAnimType::eIdleAir_Combat);
			ChangeState(EState::eIdle);
		}
		break;
	case false:	// �[��
		// �n�ʂɐڒn���Ă��邩����
		if (mIsGrounded)
		{
			// �ړ�����
			// �L�[�̓��̓x�N�g�����擾
			CVector input;
			if (CInput::Key('W'))		input.Z(-1.0f);
			else if (CInput::Key('S'))	input.Z(1.0f);
			if (CInput::Key('A'))		input.X(-1.0f);
			else if (CInput::Key('D'))	input.X(1.0f);

			// ���̓x�N�g���̒����œ��͂���Ă��邩����
			if (input.LengthSqr() > 0)
			{
				// �J�����̌����ɍ��킹���ړ��x�N�g���ɕϊ�
				CVector move = mCamForward * input.Z() + mCamSide * input.X();
				move.Y(0.0f);
				move.Normalize();

				mMoveSpeed += move * MOVE_SPEED;

				// �A�j���[�V�������I��
				if (IsAnimationFinished())
				{
					// ����A�j���[�V�����ɐ؂�ւ�
					ChangeAnimation(EAnimType::eRun);
				}
				// �_�b�V���ړ��̐؂�ւ�
				if (CInput::Key(VK_SHIFT))
				{
					// �X�^�~�i��0�ȏォ�t���O�������Ă��Ȃ�
					if (mStatus.sp > 0 && !mSPZeroFlag)
					{
						ChangeState(EState::eFastMove);
						ChangeAnimation(EAnimType::eFastRun);
					}
				}
				// ��𓮍�ւ̐؂�ւ�
				if (CInput::PushKey(VK_SPACE))
				{
					if (mStatus.sp >= AVOID_SPCOST)
					{
						ChangeState(EState::eAvoidance);
						ChangeAnimation(EAnimType::eRollStart);
					}
				}
				// ���N���b�N�܂��́A�}�E�X�z�C�[���N���b�N�Ŕ���
				if (CInput::PushKey(VK_LBUTTON) || CInput::PushKey(VK_MBUTTON))
				{
					ChangeState(EState::eMove);
					ChangeAnimation(EAnimType::eRun_Drawn_Combat);
				}
			}
			// �ړ��L�[��������Ă��Ȃ�
			else
			{
				// ����I���̃A�j���[�V�������Đ����A�A�C�h����Ԃɖ߂�
				ChangeAnimation(EAnimType::eRunEnd);
				ChangeState(EState::eIdle);
			}
		}
		else
		{
			ChangeAnimation(EAnimType::eIdleAir);
			ChangeState(EState::eIdle);
		}
		break;
	}
}

//�_�b�V���ړ�
void CPlayer::Update_FastMove()
{
	mMoveSpeed.X(0.0f);
	mMoveSpeed.Z(0.0f);

	if (!mpFastRunSE->IsPlaying())
	{
		mpFastRunSE->PlayLoop(-1, true, 0.2f);
	}
	else
	{
		mpFastRunSE->SetVolume(0.2f);
	}

	//�n�ʂɐڒn���Ă��邩����
	if (mIsGrounded)
	{
		// �ړ����� 
		// �_�b�V���L�[�̓��͔���	true:���͒� false:����͒�
		if (!mIsDash) mIsDash = true;
		if (CInput::PullKey(VK_SHIFT)) mIsDash = false;

		// �L�[�̓��̓x�N�g�����擾
		CVector input;
		if (CInput::Key('W'))		input.Z(-1.0f);
		else if (CInput::Key('S'))	input.Z(1.0f);
		if (CInput::Key('A'))		input.X(-1.0f);
		else if (CInput::Key('D'))	input.X(1.0f);

		// ���̓x�N�g���̒����œ��͂���Ă��邩����
		if (input.LengthSqr() > 0)
		{
			// �J�����̌����ɍ��킹���ړ��x�N�g���ɕϊ�
			CCamera* mainCamera = CCamera::MainCamera();
			CVector camFoward = mainCamera->VectorZ();
			CVector camSide = CVector::Cross(CVector::up, camFoward);
			CVector move = camFoward * input.Z() + camSide * input.X();
			move.Y(0.0f);
			move.Normalize();

			mMoveSpeed += move * FASTMOVE_SPEED;
			// �X�^�~�i������
			mStatus.sp -= 0.3;

			// �_�b�V���J�n�A�j���[�V�������I��
			if (IsAnimationFinished())
			{
				// �_�b�V���A�j���[�V�����ɐ؂�ւ�
				ChangeAnimation(EAnimType::eFastRun);
			}
			// ��𓮍�ւ̐؂�ւ�
			if (CInput::PushKey(VK_SPACE))
			{
				if (mStatus.sp >= AVOID_SPCOST)
				{
					ChangeState(EState::eAvoidance);
					ChangeAnimation(EAnimType::eRollStart);
					mIsDash = false;
					return;
				}
			}
			// �_�b�V���L�[�𗣂����ꍇ
			// �_�b�V���̃A�j���[�V�������瑖��A�j���[�V�����ɐ؂�ւ���
			if (!mIsDash)
			{
				ChangeState(EState::eMove);
				ChangeAnimation(EAnimType::eRun);
			}
		}
		// �ړ��L�[��������Ă��Ȃ�
		else
		{
			// ����I���̃A�j���[�V�������Đ����A�A�C�h����Ԃɖ߂�
			ChangeAnimation(EAnimType::eFastRunEnd);
			ChangeState(EState::eIdle);
			mIsDash = false;
		}
	}
	else
	{
		ChangeAnimation(EAnimType::eIdleAir);
		ChangeState(EState::eIdle);
		mIsDash = false;
	}
	// �X�^�~�i���[���ɂȂ����狭���I�Ƀ_�b�V�����I��
	// ��莞�ԃ_�b�V�����ł��Ȃ��悤�Ƀt���O�𗧂Ă�
	if (mStatus.sp <= 0)
	{
		mIsDash = false;
		mSPZeroFlag = true;
		ChangeState(EState::eMove);
		ChangeAnimation(EAnimType::eRun);
	}
}