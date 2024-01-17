#include "CPlayer.h"

// ��𓮍�
void CPlayer::Update_Avoidance()
{
	mMoveSpeed.X(0.0f);
	mMoveSpeed.Z(0.0f);

	// �L�[�̓��̓x�N�g�����擾
	CVector input;

	if (CInput::Key('W'))		input.Z(-1.0f);
	else if (CInput::Key('S'))	input.Z(1.0f);
	if (CInput::Key('A'))		input.X(-1.0f);
	else if (CInput::Key('D'))	input.X(1.0f);
	// ��𒆂̃t���O�𗧂Ă�
	if (!mIsAvoid) mIsAvoid = true;

	// ���ۑ��̓��̓x�N�g���������l�̏ꍇ
	if (mInput_save.LengthSqr() == 0.0f)
	{
		// ���̓x�N�g���f�[�^���ꎞ�I�ɕۑ�
		mInput_save = mCamForward * input.Z() + mCamSide * input.X();
	}
	// �J�����̌����ɍ��킹���ړ��x�N�g���ɕϊ�
	CVector move = mInput_save;
	move.Y(0.0f);
	move.Normalize();

	mMoveSpeed += move * ROLL_SPEED;

	// ��𓮍삪�I��������A�A�C�h����Ԃֈڍs����
	if (IsAnimationFinished())
	{
		// ��𓮍��Ɉړ��L�[���͂�����Α����Ԃֈڍs
		if (input.LengthSqr() > 0)
		{
			mState = EState::eMove;
			// �[���̔���
			if (mIsDrawn)// ����
			{
				ChangeAnimation(EAnimType::eRollEnd_run_Combat);
			}
			else// �[��
			{
				ChangeAnimation(EAnimType::eRollEnd_run);
				// �_�b�V���L�[��������Ă���ꍇ�A�_�b�V���ɐ؂�ւ���(�[�����̂�)
				if (CInput::Key(VK_SHIFT))
				{
					// �X�^�~�i��0�ȏォ�t���O�������Ă��Ȃ�
					if (mStatus.sp > 0 && !mSPZeroFlag)
					{
						mState = EState::eFastMove;
					}
					else
					{
						mState = EState::eMove;
						ChangeAnimation(EAnimType::eRollEnd_run);
					}
				}
			}
		}
		// �L�[���͂��Ȃ��ꍇ�̓A�C�h����Ԃֈڍs
		else
		{
			mState = EState::eIdle;
			// �[���̔���
			if (mIsDrawn)// ����
			{
				ChangeAnimation(EAnimType::eRollEnd_idle_Combat);
			}
			else// �[��
			{
				ChangeAnimation(EAnimType::eRollEnd_idle);
			}
		}
		// �ꎞ�I�ɕۑ��������̓x�N�g���������l�ɖ߂�
		mInput_save = CVector::zero;
		// ����t���O�����ɖ߂�
		mIsAvoid = false;
	}
}