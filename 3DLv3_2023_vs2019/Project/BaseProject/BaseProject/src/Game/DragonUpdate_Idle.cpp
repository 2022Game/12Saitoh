#include "CDragon.h"

// �ҋ@��Ԃ̍X�V����
void CDragon::UpdateIdle()
{
	// �v���C���[����������퓬��Ԃֈڍs����
	if (IsFoundPlayer())
	{
		ChangeAnimation(EDragonAnimType::eScream);
		mState = EState::eBattle;
	}
}