#include "CPlayer.h"

//移動
void CPlayer::Update_Move()
{
	mMoveSpeed.X(0.0f);
	mMoveSpeed.Z(0.0f);

	//抜納状態の判定
	switch (mIsDrawn)
	{
	case true:	// 抜刀
		// 地面に接地しているか判定
		if (mIsGrounded)
		{
			// 移動処理
			// キーの入力ベクトルを取得
			CVector input;
			if (CInput::Key('W'))		input.Z(-1.0f);
			else if (CInput::Key('S'))	input.Z(1.0f);
			if (CInput::Key('A'))		input.X(-1.0f);
			else if (CInput::Key('D'))	input.X(1.0f);

			// 入力ベクトルの長さで入力されているか判定
			if (input.LengthSqr() > 0)
			{
				// カメラの向きに合わせた移動ベクトルに変換
				CVector move = CCamera::MainCamera()->Rotation() * input;
				move.Y(0.0f);
				move.Normalize();

				mMoveSpeed += move * MOVE_SPEED;

				// アニメーションが終了
				if (IsAnimationFinished())
				{
					// 走りアニメーションに切り替え
					ChangeAnimation(EAnimType::eRun_Combat);
				}
				// 回避動作への切り替え
				if (CInput::PushKey(VK_SPACE))
				{
					mState = EState::eAvoidance;
					ChangeAnimation(EAnimType::eRollStart_Combat);
				}
				// 左クリックで攻撃状態へ移行
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
					// 弱闘技へ切り替え
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
					ChangeAnimation(EAnimType::eRun_Sheathed_Combat);
				}
			}
			// 移動キーが押されていない
			else
			{
				// 走り終わりのアニメーションを再生し、アイドル状態に戻す
				ChangeAnimation(EAnimType::eRunEnd_Combat);
				mState = EState::eIdle;
			}
		}
		else
		{
			ChangeAnimation(EAnimType::eIdleAir_Combat);
			mState = EState::eIdle;
		}
		break;
	case false:	// 納刀
		// 地面に接地しているか判定
		if (mIsGrounded)
		{
			// 移動処理
			// キーの入力ベクトルを取得
			CVector input;
			if (CInput::Key('W'))		input.Z(-1.0f);
			else if (CInput::Key('S'))	input.Z(1.0f);
			if (CInput::Key('A'))		input.X(-1.0f);
			else if (CInput::Key('D'))	input.X(1.0f);

			// 入力ベクトルの長さで入力されているか判定
			if (input.LengthSqr() > 0)
			{
				// カメラの向きに合わせた移動ベクトルに変換
				CVector move = CCamera::MainCamera()->Rotation() * input;
				move.Y(0.0f);
				move.Normalize();

				mMoveSpeed += move * MOVE_SPEED;

				// アニメーションが終了
				if (IsAnimationFinished())
				{
					// 走りアニメーションに切り替え
					ChangeAnimation(EAnimType::eRun);
				}
				// ダッシュ移動の切り替え
				if (CInput::Key(VK_SHIFT))
				{
					// スタミナが0以上かつフラグが立っていない
					if (mStatus.sp > 0 && !mSPZeroFlag)
					{
						mState = EState::eFastMove;
						ChangeAnimation(EAnimType::eFastRun);
					}
				}
				// 回避動作への切り替え
				if (CInput::PushKey(VK_SPACE))
				{
					mState = EState::eAvoidance;
					ChangeAnimation(EAnimType::eRollStart);
				}
				// 左クリックまたは、マウスホイールクリックで抜刀
				if (CInput::PushKey(VK_LBUTTON) || CInput::PushKey(VK_MBUTTON))
				{
					ChangeAnimation(EAnimType::eRun_Drawn_Combat);
				}
			}
			// 移動キーが押されていない
			else
			{
				// 走り終わりのアニメーションを再生し、アイドル状態に戻す
				ChangeAnimation(EAnimType::eRunEnd);
				mState = EState::eIdle;
			}
		}
		else
		{
			ChangeAnimation(EAnimType::eIdleAir);
			mState = EState::eIdle;
		}
		break;
	}
}

//ダッシュ移動
void CPlayer::Update_FastMove()
{
	mMoveSpeed.X(0.0f);
	mMoveSpeed.Z(0.0f);

	//地面に接地しているか判定
	if (mIsGrounded)
	{
		// 移動処理
		// キーの入力ベクトルを取得
		CVector input;
		// ダッシュキーの入力判定	true:入力中 false:非入力中
		mIsDash = true;
		if (CInput::Key('W'))		input.Z(-1.0f);
		else if (CInput::Key('S'))	input.Z(1.0f);
		if (CInput::Key('A'))		input.X(-1.0f);
		else if (CInput::Key('D'))	input.X(1.0f);
		if (CInput::PullKey(VK_SHIFT)) mIsDash = false;

		// 入力ベクトルの長さで入力されているか判定
		if (input.LengthSqr() > 0)
		{
			// カメラの向きに合わせた移動ベクトルに変換
			CVector move = CCamera::MainCamera()->Rotation() * input;
			move.Y(0.0f);
			move.Normalize();

			mMoveSpeed += move * FASTMOVE_SPEED;
			// スタミナを減少
			mStatus.sp -= 0.5;

			// ダッシュ開始アニメーションが終了
			if (IsAnimationFinished())
			{
				// ダッシュアニメーションに切り替え
				ChangeAnimation(EAnimType::eFastRun);
			}
			// 回避動作への切り替え
			if (CInput::PushKey(VK_SPACE))
			{
				mState = EState::eAvoidance;
				ChangeAnimation(EAnimType::eRollStart);
			}
			// ダッシュキーを離した場合
			// ダッシュのアニメーションから走るアニメーションに切り替える
			if (!mIsDash)
			{
				mState = EState::eMove;
				ChangeAnimation(EAnimType::eRun);
			}
		}
		// 移動キーが押されていない
		else
		{
			// 走り終わりのアニメーションを再生し、アイドル状態に戻す
			ChangeAnimation(EAnimType::eFastRunEnd);
			mState = EState::eIdle;
		}
	}
	else
	{
		ChangeAnimation(EAnimType::eIdleAir);
		mState = EState::eIdle;
	}
	// スタミナがゼロになったら強制的にダッシュを終了
	// 一定時間ダッシュができないようにフラグを立てる
	if (mStatus.sp <= 0)
	{
		mIsDash = false;
		mSPZeroFlag = true;
		mState = EState::eMove;
		ChangeAnimation(EAnimType::eRun);
	}
}