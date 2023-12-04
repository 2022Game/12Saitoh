#include "CPlayer.h"

// 待機
void CPlayer::Update_Idle()
{
	mMoveSpeed.X(0.0f);
	mMoveSpeed.Z(0.0f);

	// 抜納状態の判定
	switch (mIsDrawn)
	{
	case true:	// 抜刀時
		// 地面に接地しているか判定
		if (mIsGrounded)
		{
			// 移動キーが押されたか判定
			if (CInput::Key('W') || CInput::Key('A') || CInput::Key('S') || CInput::Key('D'))
			{
				// 走り出しのアニメーションを再生
				ChangeAnimation(EAnimType::eRunStart_Combat);
				mState = EState::eMove;
			}
			else
			{
				// アニメーションが終了したら
				// アイドルアニメーションに切り替える
				if (IsAnimationFinished())
				{
					ChangeAnimation(EAnimType::eIdle_Combat);
				}
			}
			// 左クリックで通常攻撃へ移行
			if (CInput::PushKey(VK_LBUTTON))
			{
				mState = EState::eAttack;
				ChangeAnimation(EAnimType::eNormalAttack1_1);
			}
			// マウスホイールクリックでジャンプ攻撃へ移行
			if (CInput::PushKey(VK_MBUTTON))
			{
				mState = EState::eAttack;
				ChangeAnimation(EAnimType::eAttack_Up);
			}
			// Cキー＋左クリックで弱闘技へ移行
			if (CInput::Key('C') && CInput::Key(VK_LBUTTON))
			{
				SetSpecalMove(EAnimType::eCounter_Start, false);
			}
			// Cキー＋右クリックで強闘技へ移行
			if (CInput::Key('C') && CInput::Key(VK_RBUTTON))
			{
				SetSpecalMove(EAnimType::ePowerAttack_Start, true);
			}
			// Eキーで納刀
			if (CInput::PushKey('E'))
			{
				ChangeAnimation(EAnimType::eIdle_Sheathed_Combat);
			}
		}
		else
		{
			ChangeAnimation(EAnimType::eIdleAir_Combat);
			// 一度も空中攻撃を行っていない場合、
			// 左クリックで空中攻撃へ移行する
			if (!mIsAirAttack && CInput::PushKey(VK_LBUTTON))
			{
				mState = EState::eAttack;
				ChangeAnimation(EAnimType::eAirAttack1_1);
				mIsAirAttack = true;
			}
		}
		break;
	case false:	// 納刀時
		// 地面に接地しているか判定
		if (mIsGrounded)
		{
			// 移動キーが押されたか判定
			if (CInput::Key('W') || CInput::Key('A') || CInput::Key('S') || CInput::Key('D'))
			{
				// 走り出しのアニメーションを再生
				ChangeAnimation(EAnimType::eRunStart);
				mState = EState::eMove;
				// ダッシュキーが押されている場合ダッシュ移動状態に切り替える
				if (CInput::Key(VK_SHIFT))
				{
					mState = EState::eFastMove;
					ChangeAnimation(EAnimType::eFastRunStart);
				}
			}
			else // 移動キーが押されていない時
			{
				// アニメーションが終了したら
				// アイドルアニメーションに切り替える
				if (IsAnimationFinished())
				{
					ChangeAnimation(EAnimType::eIdle);
				}
				// 左クリックまたは、マウスホイールクリックで抜刀
				if (CInput::PushKey(VK_LBUTTON) || CInput::PushKey(VK_MBUTTON))
				{
					// 抜刀アニメーションを再生
					ChangeAnimation(EAnimType::eIdle_Drawn_Combat);
				}
			}
		}
		break;
	}
}