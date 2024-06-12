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
		SetAnimationSpeed(0.5f);
		mState = EState::eBattle;
		mBatteleStep = 2;

		// モーションブラーを掛けている最中であれば、
		// 新しくモーションブラーを掛け直さない
		if (mMotionBlurRemainTime <= 0.0f)
		{
			System::SetEnableMotionBlur(true);
			mMotionBlurRemainTime = MOTION_BLUR_TIME;
		}
	}

	if (CInput::PushKey('F'))
	{
		mState = EState::eSpAttack;
		ChangeAnimation(EDragonAnimType::eScream);
		SetAnimationSpeed(0.5f);
	}
}