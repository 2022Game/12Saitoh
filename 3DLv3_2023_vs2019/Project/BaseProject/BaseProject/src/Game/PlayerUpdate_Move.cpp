#include "CPlayer.h"

//移動
void CPlayer::Update_Move()
{
	mMoveSpeed.X(0.0f);
	mMoveSpeed.Z(0.0f);

	if (!mpRunSE->IsPlaying())
	{
		mpRunSE->PlayLoop(-1, true, 0.1f);
	}
	else
	{
		mpRunSE->SetVolume(0.1f);
	}
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
				CCamera* mainCamera = CCamera::MainCamera();
				CVector camForward = mainCamera->VectorZ();
				CVector camSide = CVector::Cross(CVector::up, camForward);
				CVector move = camForward * input.Z() + camSide * input.X();
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
					if (mStatus.sp >= AVOID_SPCOST)
					{
						ChangeState(EState::eAvoidance);
						ChangeAnimation(EAnimType::eRollStart_Combat);
					}
				}
				// 左クリックで攻撃状態へ移行
				if (CInput::PushKey(VK_LBUTTON))
				{
					ChangeState(EState::eAttack);
					ChangeAnimation(EAnimType::eNormalAttack1_1);
				}
				// マウスホイールクリックでジャンプ攻撃へ移行
				if (CInput::PushKey(VK_MBUTTON))
				{
					ChangeState(EState::eAttack);
					ChangeAnimation(EAnimType::eAttack_Up);
				}
				// Eキーで納刀
				if (CInput::PushKey('E'))
				{
					ChangeState(EState::eMove);
					ChangeAnimation(EAnimType::eRun_Sheathed_Combat);
				}
				// 闘技への切り替え入力が有れば闘技へ移行
				ChangeSpMove();
			}
			// 移動キーが押されていない
			else
			{
				// 走り終わりのアニメーションを再生し、アイドル状態に戻す
				ChangeAnimation(EAnimType::eRunEnd_Combat);
				ChangeState(EState::eIdle);
			}
		}
		else
		{
			ChangeAnimation(EAnimType::eIdleAir_Combat);
			ChangeState(EState::eIdle);
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
				CVector move = mCamForward * input.Z() + mCamSide * input.X();
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
						ChangeState(EState::eFastMove);
						ChangeAnimation(EAnimType::eFastRun);
					}
				}
				// 回避動作への切り替え
				if (CInput::PushKey(VK_SPACE))
				{
					if (mStatus.sp >= AVOID_SPCOST)
					{
						ChangeState(EState::eAvoidance);
						ChangeAnimation(EAnimType::eRollStart);
					}
				}
				// 左クリックまたは、マウスホイールクリックで抜刀
				if (CInput::PushKey(VK_LBUTTON) || CInput::PushKey(VK_MBUTTON))
				{
					ChangeState(EState::eMove);
					ChangeAnimation(EAnimType::eRun_Drawn_Combat);
				}
			}
			// 移動キーが押されていない
			else
			{
				// 走り終わりのアニメーションを再生し、アイドル状態に戻す
				ChangeAnimation(EAnimType::eRunEnd);
				ChangeState(EState::eIdle);
			}
		}
		else
		{
			ChangeAnimation(EAnimType::eIdleAir);
			ChangeState(EState::eIdle);
		}
		break;
	}
}

//ダッシュ移動
void CPlayer::Update_FastMove()
{
	mMoveSpeed.X(0.0f);
	mMoveSpeed.Z(0.0f);

	if (!mpFastRunSE->IsPlaying())
	{
		mpFastRunSE->PlayLoop(-1, true, 0.2f);
	}
	else
	{
		mpFastRunSE->SetVolume(0.2f);
	}

	//地面に接地しているか判定
	if (mIsGrounded)
	{
		// 移動処理 
		// ダッシュキーの入力判定	true:入力中 false:非入力中
		if (!mIsDash) mIsDash = true;
		if (CInput::PullKey(VK_SHIFT)) mIsDash = false;

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
			CCamera* mainCamera = CCamera::MainCamera();
			CVector camFoward = mainCamera->VectorZ();
			CVector camSide = CVector::Cross(CVector::up, camFoward);
			CVector move = camFoward * input.Z() + camSide * input.X();
			move.Y(0.0f);
			move.Normalize();

			mMoveSpeed += move * FASTMOVE_SPEED;
			// スタミナを減少
			mStatus.sp -= 0.3;

			// ダッシュ開始アニメーションが終了
			if (IsAnimationFinished())
			{
				// ダッシュアニメーションに切り替え
				ChangeAnimation(EAnimType::eFastRun);
			}
			// 回避動作への切り替え
			if (CInput::PushKey(VK_SPACE))
			{
				if (mStatus.sp >= AVOID_SPCOST)
				{
					ChangeState(EState::eAvoidance);
					ChangeAnimation(EAnimType::eRollStart);
					mIsDash = false;
					return;
				}
			}
			// ダッシュキーを離した場合
			// ダッシュのアニメーションから走るアニメーションに切り替える
			if (!mIsDash)
			{
				ChangeState(EState::eMove);
				ChangeAnimation(EAnimType::eRun);
			}
		}
		// 移動キーが押されていない
		else
		{
			// 走り終わりのアニメーションを再生し、アイドル状態に戻す
			ChangeAnimation(EAnimType::eFastRunEnd);
			ChangeState(EState::eIdle);
			mIsDash = false;
		}
	}
	else
	{
		ChangeAnimation(EAnimType::eIdleAir);
		ChangeState(EState::eIdle);
		mIsDash = false;
	}
	// スタミナがゼロになったら強制的にダッシュを終了
	// 一定時間ダッシュができないようにフラグを立てる
	if (mStatus.sp <= 0)
	{
		mIsDash = false;
		mSPZeroFlag = true;
		ChangeState(EState::eMove);
		ChangeAnimation(EAnimType::eRun);
	}
}