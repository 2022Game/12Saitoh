#include "CDragon.h"
#include "CInput.h"
#include "CColliderLine.h"

// �ҋ@��Ԃ̍X�V����
void CDragon::UpdateIdle()
{
	mMoveSpeed.X(0.0f);
	mMoveSpeed.Z(0.0f);
	// �v���C���[����������퓬��Ԃֈڍs����
	if (IsFoundPlayer())
	{
		ChangeAnimation(EDragonAnimType::eScream);
		mState = EState::eBattle;
		mBatteleStep = 2;
	}

	if (IsAnimationFinished())
	{
		ChangeAnimation(EDragonAnimType::eIdle1);
	}
	if (CInput::Key(VK_UP))
	{
		mMoveSpeed += CVector(2.0f, 0.0f, 3.0f);
	}

	if (CInput::PushKey('J'))
	{
		ChangeAnimation(EDragonAnimType::eBackStep);
		mBatteleStep = 2;
	}
	if (AnimationIndex() == (int)EDragonAnimType::eBackStep)
	{
		// �W�����v����
		if (GetAnimationFrame() == 33.0f)
		{
			mMoveSpeed += CVector(0.0f, 3.5f, 0.0f);
			mIsGrounded = false;
		}

		// �ړ�����
		if (33.0f <= GetAnimationFrame() &&
			GetAnimationFrame() <= 65.0f)
		{
			mMoveSpeed += -VectorZ() * 4.0;
		}
		if (42.0f <= GetAnimationFrame() &&
			GetAnimationFrame() <= 60.0f)
		{
			mMoveSpeed += CVector(0.0f, -0.33f, 0.0f);
		}

		// �R���C�_�[����
		if (GetAnimationFrame() >= 0.0f)
		{
			mpColliderLine->Position(0.0f, 13.0f, 0.0f);
		}
		if (GetAnimationFrame() >= 60.0f)
		{
			mpColliderLine->Position(CVector::zero);
		}
	}
}