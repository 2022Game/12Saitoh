#include "CPlayer.h"

// �ҋ@
void CPlayer::Update_Idle()
{
	mMoveSpeed.X(0.0f);
	mMoveSpeed.Z(0.0f);

	// ���[��Ԃ̔���
	switch (mIsDrawn)
	{
	case true:	// ������
		// �n�ʂɐڒn���Ă��邩����
		if (mIsGrounded)
		{
			// �ړ��L�[�������ꂽ������
			if (CInput::Key('W') || CInput::Key('A') || CInput::Key('S') || CInput::Key('D'))
			{
				// ����o���̃A�j���[�V�������Đ�
				ChangeAnimation(EAnimType::eRunStart_Combat);
				mState = EState::eMove;
			}
			else
			{
				// �A�j���[�V�������I��������
				// �A�C�h���A�j���[�V�����ɐ؂�ւ���
				if (IsAnimationFinished())
				{
					ChangeAnimation(EAnimType::eIdle_Combat);
				}
			}
			// ���N���b�N�Œʏ�U���ֈڍs
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
			// C�L�[�{���N���b�N�Ŏ㓬�Z�ֈڍs
			if (CInput::Key('C') && CInput::Key(VK_LBUTTON))
			{
				SetSpecalMove(EAnimType::eCounter_Start, false);
			}
			// C�L�[�{�E�N���b�N�ŋ����Z�ֈڍs
			if (CInput::Key('C') && CInput::Key(VK_RBUTTON))
			{
				SetSpecalMove(EAnimType::ePowerAttack_Start, true);
			}
			// E�L�[�Ŕ[��
			if (CInput::PushKey('E'))
			{
				ChangeAnimation(EAnimType::eIdle_Sheathed_Combat);
			}
		}
		else
		{
			ChangeAnimation(EAnimType::eIdleAir_Combat);
			// ��x���󒆍U�����s���Ă��Ȃ��ꍇ�A
			// ���N���b�N�ŋ󒆍U���ֈڍs����
			if (!mIsAirAttack && CInput::PushKey(VK_LBUTTON))
			{
				mState = EState::eAttack;
				ChangeAnimation(EAnimType::eAirAttack1_1);
				mIsAirAttack = true;
			}
		}
		break;
	case false:	// �[����
		// �n�ʂɐڒn���Ă��邩����
		if (mIsGrounded)
		{
			// �ړ��L�[�������ꂽ������
			if (CInput::Key('W') || CInput::Key('A') || CInput::Key('S') || CInput::Key('D'))
			{
				// ����o���̃A�j���[�V�������Đ�
				ChangeAnimation(EAnimType::eRunStart);
				mState = EState::eMove;
				// �_�b�V���L�[��������Ă���ꍇ�_�b�V���ړ���Ԃɐ؂�ւ���
				if (CInput::Key(VK_SHIFT))
				{
					mState = EState::eFastMove;
					ChangeAnimation(EAnimType::eFastRunStart);
				}
			}
			else // �ړ��L�[��������Ă��Ȃ���
			{
				// �A�j���[�V�������I��������
				// �A�C�h���A�j���[�V�����ɐ؂�ւ���
				if (IsAnimationFinished())
				{
					ChangeAnimation(EAnimType::eIdle);
				}
				// ���N���b�N�܂��́A�}�E�X�z�C�[���N���b�N�Ŕ���
				if (CInput::PushKey(VK_LBUTTON) || CInput::PushKey(VK_MBUTTON))
				{
					// �����A�j���[�V�������Đ�
					ChangeAnimation(EAnimType::eIdle_Drawn_Combat);
				}
			}
		}
		break;
	}
}