#include "CDragon.h"
#include "CInput.h"
#include "CColliderLine.h"

// 待機状態の更新処理
void CDragon::UpdateIdle()
{
	mMoveSpeed.X(0.0f);
	mMoveSpeed.Z(0.0f);
	// プレイヤーを見つけたら戦闘状態へ移行する
	if (IsFoundPlayer())
	{
		ChangeAnimation(EDragonAnimType::eScream);
		mState = EState::eBattle;
		mBatteleStep = 2;
	}

	if (CInput::PushKey('J'))
	{
		ChangeAnimation(EDragonAnimType::eBackStep);
		mBatteleStep = 2;
	}
	if (AnimationIndex() == (int)EDragonAnimType::eBackStep)
	{
		

	}
}