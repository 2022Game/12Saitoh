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

	if (CInput::PushKey('F'))
	{
		mState = EState::eSpAttack;
		ChangeAnimation(EDragonAnimType::eScream);
	}
}