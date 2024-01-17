#include "CPlayer.h"

// 回避動作
void CPlayer::Update_Avoidance()
{
	mMoveSpeed.X(0.0f);
	mMoveSpeed.Z(0.0f);

	// キーの入力ベクトルを取得
	CVector input;

	if (CInput::Key('W'))		input.Z(-1.0f);
	else if (CInput::Key('S'))	input.Z(1.0f);
	if (CInput::Key('A'))		input.X(-1.0f);
	else if (CInput::Key('D'))	input.X(1.0f);
	// 回避中のフラグを立てる
	if (!mIsAvoid) mIsAvoid = true;

	// 仮保存の入力ベクトルが初期値の場合
	if (mInput_save.LengthSqr() == 0.0f)
	{
		// 入力ベクトルデータを一時的に保存
		mInput_save = mCamForward * input.Z() + mCamSide * input.X();
	}
	// カメラの向きに合わせた移動ベクトルに変換
	CVector move = mInput_save;
	move.Y(0.0f);
	move.Normalize();

	mMoveSpeed += move * ROLL_SPEED;

	// 回避動作が終了したら、アイドル状態へ移行する
	if (IsAnimationFinished())
	{
		// 回避動作後に移動キー入力があれば走り状態へ移行
		if (input.LengthSqr() > 0)
		{
			mState = EState::eMove;
			// 納抜の判定
			if (mIsDrawn)// 抜刀
			{
				ChangeAnimation(EAnimType::eRollEnd_run_Combat);
			}
			else// 納刀
			{
				ChangeAnimation(EAnimType::eRollEnd_run);
				// ダッシュキーが押されている場合、ダッシュに切り替える(納刀中のみ)
				if (CInput::Key(VK_SHIFT))
				{
					// スタミナが0以上かつフラグが立っていない
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
		// キー入力がない場合はアイドル状態へ移行
		else
		{
			mState = EState::eIdle;
			// 納抜の判定
			if (mIsDrawn)// 抜刀
			{
				ChangeAnimation(EAnimType::eRollEnd_idle_Combat);
			}
			else// 納刀
			{
				ChangeAnimation(EAnimType::eRollEnd_idle);
			}
		}
		// 一時的に保存した入力ベクトルを初期値に戻す
		mInput_save = CVector::zero;
		// 回避フラグを元に戻す
		mIsAvoid = false;
	}
}