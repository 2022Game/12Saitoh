#include "CDragon.h"

// 待機状態の更新処理
void CDragon::UpdateIdle()
{
	// プレイヤーを見つけたら戦闘状態へ移行する
	if (IsFoundPlayer())
	{
		ChangeAnimation(EDragonAnimType::eScream);
		mState = EState::eBattle;
	}
}