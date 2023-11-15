#include "CPlayer.h"

//�ړ�
void CPlayer::Update_Move()
{
	mMoveSpeed.X(0.0f);
	mMoveSpeed.Z(0.0f);

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
				CVector move = CCamera::MainCamera()->Rotation() * input;
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
					mState = EState::eAvoidance;
					ChangeAnimation(EAnimType::eRollStart_Combat);
				}
				// ���N���b�N�ōU����Ԃֈڍs
				if (CInput::PushKey(VK_LBUTTON))
				{
					mState = EState::eAttack;
					ChangeAnimation(EAnimType::eNormalAttack1_1);
				}
				// �}�E�X�z�C�[���N���b�N�ŃW�����v�U���ֈڍs
				if (CInput::PushKey(VK_MBUTTON))
				{
					mState = EState::eAttack;
					ChangeAnimation(EAnimType::eAttack_Up);
				}
				// E�L�[�Ŕ[��
				if (CInput::PushKey('E'))
				{
					ChangeAnimation(EAnimType::eRun_Sheathed_Combat);
				}
			}
			// �ړ��L�[��������Ă��Ȃ�
			else
			{
				// ����I���̃A�j���[�V�������Đ����A�A�C�h����Ԃɖ߂�
				ChangeAnimation(EAnimType::eRunEnd_Combat);
				mState = EState::eIdle;
			}
		}
		else
		{
			ChangeAnimation(EAnimType::eIdleAir_Combat);
			mState = EState::eIdle;
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
				CVector move = CCamera::MainCamera()->Rotation() * input;
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
					mState = EState::eFastMove;
					ChangeAnimation(EAnimType::eFastRun);
				}
				// ��𓮍�ւ̐؂�ւ�
				if (CInput::PushKey(VK_SPACE))
				{
					mState = EState::eAvoidance;
					ChangeAnimation(EAnimType::eRollStart);
				}
				// ���N���b�N�܂��́A�}�E�X�z�C�[���N���b�N�Ŕ���
				if (CInput::PushKey(VK_LBUTTON) || CInput::PushKey(VK_MBUTTON))
				{
					ChangeAnimation(EAnimType::eRun_Drawn_Combat);
				}
			}
			// �ړ��L�[��������Ă��Ȃ�
			else
			{
				// ����I���̃A�j���[�V�������Đ����A�A�C�h����Ԃɖ߂�
				ChangeAnimation(EAnimType::eRunEnd);
				mState = EState::eIdle;
			}
		}
		else
		{
			ChangeAnimation(EAnimType::eIdleAir);
			mState = EState::eIdle;
		}
		break;
	}
}

//�_�b�V���ړ�
void CPlayer::Update_FastMove()
{
	mMoveSpeed.X(0.0f);
	mMoveSpeed.Z(0.0f);

	//�n�ʂɐڒn���Ă��邩����
	if (mIsGrounded)
	{
		// �ړ�����
		// �L�[�̓��̓x�N�g�����擾
		CVector input;
		// �_�b�V���L�[�̓��͔���	true:���͒� false:����͒�
		bool dash = true;
		if (CInput::Key('W'))		input.Z(-1.0f);
		else if (CInput::Key('S'))	input.Z(1.0f);
		if (CInput::Key('A'))		input.X(-1.0f);
		else if (CInput::Key('D'))	input.X(1.0f);
		if (CInput::PullKey(VK_SHIFT)) dash = false;

		// ���̓x�N�g���̒����œ��͂���Ă��邩����
		if (input.LengthSqr() > 0)
		{
			// �J�����̌����ɍ��킹���ړ��x�N�g���ɕϊ�
			CVector move = CCamera::MainCamera()->Rotation() * input;
			move.Y(0.0f);
			move.Normalize();

			mMoveSpeed += move * FASTMOVE_SPEED;

			// �_�b�V���J�n�A�j���[�V�������I��
			if (IsAnimationFinished())
			{
				// �_�b�V���A�j���[�V�����ɐ؂�ւ�
				ChangeAnimation(EAnimType::eFastRun);
			}
			// ��𓮍�ւ̐؂�ւ�
			if (CInput::PushKey(VK_SPACE))
			{
				mState = EState::eAvoidance;
				ChangeAnimation(EAnimType::eRollStart);
			}
			// �_�b�V���L�[�𗣂����ꍇ
			// �_�b�V���̃A�j���[�V�������瑖��A�j���[�V�����ɐ؂�ւ���
			if (dash == false)
			{
				mState = EState::eMove;
				ChangeAnimation(EAnimType::eRun);
			}
		}
		// �ړ��L�[��������Ă��Ȃ�
		else
		{
			// ����I���̃A�j���[�V�������Đ����A�A�C�h����Ԃɖ߂�
			ChangeAnimation(EAnimType::eFastRunEnd);
			mState = EState::eIdle;
		}
	}
	else
	{
		ChangeAnimation(EAnimType::eIdleAir);
		mState = EState::eIdle;
	}
}