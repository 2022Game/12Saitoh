#include "CDragon.h"
#include "CInput.h"
#include "CColliderLine.h"
#include "CHPGauge_Dragon.h"

// 待機状態の更新処理
void CDragon::UpdateIdle()
{
	mMoveSpeed.X(0.0f);
	mMoveSpeed.Z(0.0f);
	// プレイヤーを見つけたら戦闘状態へ移行する
	if (IsFoundPlayer())
	{
		ChangeAnimation(EDragonAnimType::eScream);
		SetAnimationSpeed(0.5f);
		mState = EState::eBattle;
		mBatteleStep = 2;

		//HPゲージを作成
		mpHPGauge = new CHPGauge_Dragon();
		mpHPGauge->SetPos(250.0f, 600.0f);
		mpHPGauge->SetMaxValue(mStatus.hp);
	}
}