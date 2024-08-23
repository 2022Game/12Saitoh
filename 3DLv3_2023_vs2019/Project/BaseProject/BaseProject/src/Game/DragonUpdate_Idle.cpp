#include "CDragon.h"
#include "CInput.h"
#include "CColliderLine.h"
#include "CHPGauge_Dragon.h"

// �ҋ@��Ԃ̍X�V����
void CDragon::UpdateIdle()
{
	mMoveSpeed.X(0.0f);
	mMoveSpeed.Z(0.0f);
	// �v���C���[����������퓬��Ԃֈڍs����
	if (IsFoundPlayer())
	{
		ChangeAnimation(EDragonAnimType::eScream);
		SetAnimationSpeed(0.5f);
		mState = EState::eBattle;
		mBatteleStep = 2;

		//HP�Q�[�W���쐬
		mpHPGauge = new CHPGauge_Dragon();
		mpHPGauge->SetPos(250.0f, 600.0f);
		mpHPGauge->SetMaxValue(mStatus.hp);
	}
}