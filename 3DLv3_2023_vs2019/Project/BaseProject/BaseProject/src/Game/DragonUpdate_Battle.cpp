#include "CDragon.h"
#include "Maths.h"
#include "Time.h"
#include "CColliderLine.h"
#include "CPlayer.h"
#include "CFlamethrower.h"
#include "Global.h"

// 戦闘中の更新処理
void CDragon::UpdateBattle()
{
	mMoveSpeed.X(0.0f);
	mMoveSpeed.Z(0.0f);

	// 戦闘の段階によって処理を変更
	switch (mBatteleStep)
	{
		/* 0～2 : 戦闘時の基本的な処理 */
	case 0:// 戦闘中のアイドル処理
		UpdateBattele_Idle();
		// 怒り状態への移行処理
		if (mChangeAngry) ChangeAngry();
		break;
	case 1:// 戦闘中の追跡処理
		UpdateBattle_Chase();
		break;
	case 2:// 攻撃処理
		UpdateAttack();
		break;
		
		/* 3以降 : 戦闘時の特別な処理 */
	case 3:// 怯み処理
		UpdateBattele_Fear();
		break;
	}

	// 怯み値が最大値以上になった場合
	// 怯みモーションを行う
	if (mFearValue >= FEAR_MAXVALUE)
	{
		// 各値を初期化
		mRandSave = 0;
		mAttackStep = 0;
		mElapsedTime = 0.0f;
		mChaseElapsedTime = 0.0f;
		mpFlamethrower->Stop();
		mpColliderLine->Position(CVector::zero);

		// 怯み値は0する
		mFearValue = 0;
		mBatteleStep = 3;
		ChangeAnimation(EDragonAnimType::eGetHit);
		SetAnimationSpeed(0.3);
	}

	// 怒り状態のとき
	if (mIsAngry)
	{
		// 怒り状態になってからの時間を取得
		mAngryElapsedTime += Time::DeltaTime();

		// 怒り状態になってから1分経過で怒り状態を解除
		if (mAngryElapsedTime >= 60.0f)
		{
			mIsAngry = false;
			mAngryValue = 0;
			mAngryElapsedTime = 0.0f;
			// 各ステータスを元に戻す
			mStatus.atk = mMaxStatus.atk;
			mStatus.def = mMaxStatus.def;
		}
		// 怒り値が0以下になった場合、怒り状態を解除
		else if (mAngryValue < 0)
		{
			mIsAngry = false;
			mAngryValue = 0;
			mAngryElapsedTime = 0.0f;
			// 各ステータスを元に戻す
			mStatus.atk = mMaxStatus.atk;
			mStatus.def = mMaxStatus.def;
		}
	}
	// 非怒り状態のとき
	else
	{
		// 怒り値が基準より大きくなった場合、怒り状態へ移行
		if (mAngryValue >= mAngryStandardValue)
		{
			// 怒り状態への移行フラグを立てる
			mChangeAngry = true;
		}
	}
}

// 戦闘中のアイドル処理
void CDragon::UpdateBattele_Idle()
{
	// 必殺技攻撃への移行処理
	// HPが50%以下かつ、一度も必殺技を撃っていない
	if (GetHPPercent() <= HPPER_50 && mSpAttackNum == 0)
	{
		mSpAttackNum++;
		mState = EState::eSpAttack;
		ChangeAnimation(EDragonAnimType::eScream);
		SetAnimationSpeed(0.5f);
		return;
	}
	// HPが25%以下になったらもう一度撃つ
	else if (GetHPPercent() <= HPPER_25 && mSpAttackNum == 1)
	{
		mSpAttackNum++;
		mState = EState::eSpAttack;
		ChangeAnimation(EDragonAnimType::eScream);
		SetAnimationSpeed(0.5f);
		return;
	}

	// 経過時間を加算する
	mElapsedTime += Time::DeltaTime();
	// 1.5秒ごとに攻撃を行う
	if (mElapsedTime >= 1.0f)
	{
		// 経過時間を初期化
		mElapsedTime = 0.0f;
		mRandSave = 0;
		mBatteleStep++;

		// プレイヤーとの距離を取得
		PlayerFromDistance();
		return;
	}

	// 保存用の変数に生成した乱数を代入して。
	// 生成した乱数に応じて処理を実行
	if (mRandSave == 0) mRandSave = BattleIdleRand();

	switch (mRandSave)
	{
	case 1: // その場で待機モーションを行う
		ChangeAnimation(EDragonAnimType::eIdle1);
		SetAnimationSpeed(0.4f);
		break;

	case 2:// プレイヤーの元へ移動する
	{
		CVector playerPos = CPlayer::Instance()->Position();
		CVector enemyPos = Position();
		// 自身からプレイヤーまでのベクトルを取得
		CVector EP = (playerPos - enemyPos).Normalized();
		EP.Y(0.0f);
		mMoveSpeed += EP * 2.0f;
		ChangeAnimation(EDragonAnimType::eWalk);
		SetAnimationSpeed(0.7f);
	}
		break;

	case 3:// 攻撃を行う

		// 各変数を初期化
		mElapsedTime = 0.0f;
		mRandSave = 0;
		mBatteleStep++;
		// プレイヤーとの距離を取得
		PlayerFromDistance();
		break;
	}
}

// 戦闘中の追跡処理
void CDragon::UpdateBattle_Chase()
{
	CVector playerPos = CPlayer::Instance()->Position();
	CVector enemyPos = Position();

	// 距離に応じて処理を行う
	switch (mDistanceType)
	{
	case EDistanceType::eNear:// 近距離
	{
		CVector forward = VectorZ().Normalized();
		// 自身からプレイヤーまでのベクトルを取得
		CVector EP = (playerPos - enemyPos).Normalized();
		EP.Y(0.0f);
		// 正面方向のベクトルとプレイヤーまでの内積から角度を求める
		float dotZ = forward.Dot(EP);
		// 自身からプレイヤーまでのベクトルの長さを取得
		float distance = (playerPos - enemyPos).Length();

		switch (mRandSave)
		{
		case 1:// 噛みつき攻撃の範囲まで移動して攻撃を行う
			// 噛みつき攻撃の範囲内の場合
			if (distance <= 120.0f)
			{
				// 噛みつき攻撃を行う
				ChangeAnimation(EDragonAnimType::eAttackMouth);
				SetAnimationSpeed(0.43);
				mBatteleStep++;
				mChaseElapsedTime = 0.0f;
			}
			else
			{
				// 攻撃ができる範囲まで移動
				mMoveSpeed += EP * 0.7f;
				ChangeAnimation(EDragonAnimType::eWalk);
				SetAnimationSpeed(0.7f);
				// 移動時間を加算
				mChaseElapsedTime += Time::DeltaTime();
			}

			// 2.5秒以上移動した場合、中距離攻撃に移行する
			if (mChaseElapsedTime >= 2.5f)
			{
				mChaseElapsedTime = 0.0f;
				mDistanceType = EDistanceType::eMedium;
			}
			break;

		case 2:// バックステップを行い、その後攻撃に移行

			// バックステップ処理
			if (AnimationIndex() == (int)EDragonAnimType::eBackStep)
			{
				Update_BackStep();
			}
			// バックステップが終了したら
			if (IsAnimationFinished())
			{
				// 乱数を生成して、攻撃を決定する
				int rand = Math::Rand(0, 3);
				if (rand <= 2)
				{
					// 中距離攻撃に切り替え
					mDistanceType = EDistanceType::eMedium;
					mRandSave = 0;
				}
				else
				{
					// 一定の範囲まで移動して攻撃を行う
					mRandSave = 1;
				}
			}
			break;
		case 3:// 乱数を生成して行動を決定
		{
			CVector fPos = gField->Position();
			CVector dPos = Position();
			float length = (fPos - dPos).Length();

			// ドラゴンがフィールドの中心から半径の半分以上離れていたら
			// 移動または攻撃を行う
			if (length >= FIELD_RADIUS / 1.5)
			{
				int rand = Math::Rand(0, 2);
				if (rand <= 1)
				{
					// 移動
					ChangeAnimation(EDragonAnimType::eRun);
					SetAnimationSpeed(0.5f);
					// 移動後、攻撃を行う
					mRandSave = 4;
					mRayAngle = GetRayAngle();
				}
				else if (rand == 2)
				{
					// バックステップ
					if (IsBackStep())
					{
						ChangeAnimation(EDragonAnimType::eBackStep);
						SetAnimationSpeed(0.5f);
						mRandSave = 2;
					}
					// バックステップが出来ない
					else
					{
						ChangeAnimation(EDragonAnimType::eWalk);
						// 移動後、攻撃を行う
						mRandSave = 4;
						mRayAngle = GetRayAngle();
					}

				}
			}
			// 半分以上離れていない場合はバックステップを行う
			else
			{
				// バックステップ
				if (IsBackStep())
				{
					ChangeAnimation(EDragonAnimType::eBackStep);
					SetAnimationSpeed(0.5f);
					mRandSave = 2;
				}
				else
				{
					// 中距離攻撃に切り替え
					mDistanceType = EDistanceType::eMedium;
					mRandSave = 0;
				}
			}
			break;
		}
		case 4:// 移動
		{
			// 求めた角度の方向へ移動する
			CVector angle = CVector	(
				sinf(mRayAngle * M_PI / 360.0f), 
				0.0f, 
				cosf(mRayAngle * M_PI / 360.0f)).Normalized();

			mMoveSpeed += angle * 2.5f;

			mChaseElapsedTime += Time::DeltaTime();
			if (mChaseElapsedTime >= 3.0f)
			{
				mChaseElapsedTime = 0.0f;
				mRandSave = 5;
			}
		}
			break;
		case 5:// 攻撃を行う
		{
			CVector pPos = CPlayer::Instance()->Position();
			CVector dPos = Position();
			CVector PD = (pPos - dPos).Normalized();
			
			mMoveSpeed += PD * 1.0f;
			mChaseElapsedTime += Time::DeltaTime();
			if (mChaseElapsedTime >= 2.5f)
			{
				int rand = Math::Rand(0, 1);
				if (rand == 0)
				{
					ChangeAnimation(EDragonAnimType::eAttackFlame);
					SetAnimationSpeed(0.33f);
					mBatteleStep++;
					mChaseElapsedTime = 0.0f;
				}
				else
				{
					ChangeAnimation(EDragonAnimType::eAttackHand);
					SetAnimationSpeed(0.5f);
					mBatteleStep++;
					mChaseElapsedTime = 0.0f;
				}

			}
		}
			break;
		default:// 最初に一度だけ実行
			// 噛みつき攻撃の範囲外だった場合
			if (distance >= 150.0f)
			{
				// 噛みつき攻撃の範囲まで移動して攻撃を行う
				mRandSave = 1;
			}
			// 噛みつき範囲内だった場合
			else if ( 80.0f <= distance && distance <= 150.0f)
			{
				if (!(dotZ <= cosf(15 * M_PI / 180.0f)))
				{
					ChangeAnimation(EDragonAnimType::eAttackMouth);
					SetAnimationSpeed(0.43f);
					mBatteleStep++;
					mChaseElapsedTime = 0.0f;
				}
				else
				{
					if (IsBackStep())
					{
						// バックステップを行い、その後攻撃
						mRandSave = 2;
						ChangeAnimation(EDragonAnimType::eBackStep);
						SetAnimationSpeed(0.5f);
					}
					else
					{
						mRandSave = 3;
					}
				}
			}
			else// それ以外の場合
			{
				// 攻撃または移動を行う
				mRandSave = 3;
			}
			break;
		}
		break;
	}
	case EDistanceType::eMedium:// 中距離
	{
		// 自身からプレイヤーまでのベクトルを取得
		CVector EP = (playerPos - enemyPos).Normalized();
		EP.Y(0.0f);
		CVector forward = VectorZ().Normalized();
		// 正面方向のベクトルとプレイヤーまでの
		// 内積から角度を求める
		float dotZ = forward.Dot(EP);

		// 求めた角度が視野角度内
		// もしくは、1.5秒以上経過で攻撃
		if (mChaseElapsedTime >= 1.5f || !(dotZ <= cosf(10 * M_PI / 180.0f)))
		{
			// ランダム値を生成して、行う攻撃を選択
			switch (Math::Rand(0, 1))
			{
			case 0:// ブレス攻撃
				ChangeAnimation(EDragonAnimType::eAttackFlame);
				SetAnimationSpeed(0.33f);
				mBatteleStep++;
				mChaseElapsedTime = 0.0f;
				break;
			case 1:// 飛び掛かり攻撃
				ChangeAnimation(EDragonAnimType::eAttackHand);
				SetAnimationSpeed(0.5f);
				mBatteleStep++;
				mChaseElapsedTime = 0.0f;
				break;
			}
		}
		// 視野範囲外
		else
		{
			// プレイヤーの方向を向く
			mMoveSpeed += EP * 0.2f;
			mChaseElapsedTime += Time::DeltaTime();
			ChangeAnimation(EDragonAnimType::eWalk);
			SetAnimationSpeed(0.7f);
		}

		break;
	}
	case EDistanceType::eFar:// 遠距離
	{
		// 乱数保存用の変数が初期値の場合、
		// 乱数を生成して代入する
		if (mRandSave == 0) mRandSave = Math::Rand(1, 2);

		switch (mRandSave)
		{
		case 1:
		case 2:// 一定の範囲まで移動
		{
			// 自身からプレイヤーまでのベクトルの長さを取得
			float distance = (playerPos - enemyPos).Length();

			// 移動
			if (distance >= 230.0f)
			{
				// 自身からプレイヤーまでのベクトルを取得
				CVector EP = (playerPos - enemyPos).Normalized();
				EP.Y(0.0f);
				// 一定の範囲まで移動する
				mMoveSpeed += EP * 1.0f;
				ChangeAnimation(EDragonAnimType::eWalk);
				SetAnimationSpeed(0.7f);
				// 移動時間を加算
				mChaseElapsedTime += Time::DeltaTime();
			}
			// 一定の範囲まで移動したら
			else
			{
				// 乱数を保存した変数を初期化
				mRandSave = 0;
				// ランダム値を生成して、攻撃を決める
				int rand = Math::Rand(0, 1);
				// 近距離
				if (rand == 0) mDistanceType = EDistanceType::eNear;
				// 中距離
				else
				{
					mDistanceType = EDistanceType::eMedium;
					// 中距離の処理は移動が無いので、移動時間を初期化
					mChaseElapsedTime = 0.0f;
				}
			}

			// 3秒以上移動したら、攻撃を行う
			if (mChaseElapsedTime >= 3.0f)
			{
				// 乱数を保存した変数を初期化
				mRandSave = 0;
				// ブレス攻撃を行う
				ChangeAnimation(EDragonAnimType::eAttackFlame);
				SetAnimationSpeed(0.33f);
				// 移動時間の初期化
				mChaseElapsedTime = 0.0f;
				mBatteleStep++;
			}
		}
		break;
		}
	}
	}
}

// 攻撃処理
void CDragon::UpdateAttack()
{
	// 各アニメーションごとに処理を行う
	switch (AnimationIndex())
	{
	case (int)EDragonAnimType::eScream:// 咆哮
		Update_Sceream();
		break;
	case (int)EDragonAnimType::eAttackMouth:// 噛みつき攻撃
		Update_AttackMouth();
		break;
	case (int)EDragonAnimType::eAttackHand:// 飛び掛かり攻撃
		Update_AttackHand();
		break;
	case (int)EDragonAnimType::eAttackFlame:// 地上ブレス攻撃
		Update_AttackFlame();
		break;
	}

	// 攻撃が終了したらアイドルに移行する
	if (IsAnimationFinished())
	{
		mAttackStep = 0;
		mBatteleStep = 0;
		// ブレス攻撃をしていたら、ブレスを止める
		if (mpFlamethrower->IsThrowing())mpFlamethrower->Stop();
	}
}

// 怯み中の処理
void CDragon::UpdateBattele_Fear()
{
	switch (mFearStep)
	{
	case 0:// 怯み後の処理を決定

		// 怯みアニメーションが終了
		if (IsAnimationFinished())
		{
			// 残りHP割合に応じて処理を変える
			if (GetHPPercent() <= 30)
			{
				// 残りHPが30%以下の場合は、一定時間アイドルを行う
				ChangeAnimation(EDragonAnimType::eIdle1);
				SetAnimationSpeed(0.37f);
				mFearStep++;
			}
			else
			{
				// 残りHPが30%以上の場合は、戦闘へ戻る
				mBatteleStep = 0;
			}
		}
		break;
	case 1:// 一定時間アイドルを行う
		
		// 3秒経過で戦闘へ戻る
		mFearElapsedTime += Time::DeltaTime();
		if (mFearElapsedTime >= 4.0f)
		{
			mFearStep = 0;
			// 戦闘中アイドルへ戻る
			mBatteleStep = 0;
		}
		break;
	}
}

// 咆哮攻撃処理
void CDragon::Update_Sceream()
{
	// 咆哮用のコライダの切り替え
	// 攻撃段階に応じて処理を実行
	switch (mAttackStep)
	{
	case 0:// モーションブラーを実行
		if (GetAnimationFrame() >= SCREAMBLUR_START)
		{
			// モーションブラーを掛ける
			System::SetEnableMotionBlur(true);
			mMotionBlurRemainTime = MOTION_BLUR_TIME;
			AttackStart();
			// 次の段階へ移行
			mAttackStep++;
		}
		break;
	case 1:
		if (SCREAMCOL_END <= GetAnimationFrame())
		{
			AttackEnd();
		}

		if (IsAnimationFinished())
		{
			mAttackStep = 0;
		}
		break;
	}
}

// 噛みつき攻撃処理
void CDragon::Update_AttackMouth()
{
	switch (mAttackStep)
	{
	case 0:
		// 攻撃開始の合図をコライダーに伝える
		if (ATTACKMOUTH_COL_START <= GetAnimationFrame())
		{
			AttackStart();
			mAttackStep++;
		}
		break;
	case 1:
		// 攻撃終了の合図をコライダーに伝える
		if (ATTACKMOUTH_COL_END <= GetAnimationFrame())
		{
			AttackEnd();
		}
		if (IsAnimationFinished())
		{
			mAttackStep = 0;
		}
		break;
	}
}

// 飛び掛かり攻撃処理
void CDragon::Update_AttackHand()
{
	// 攻撃段階に応じて処理を実行
	switch (mAttackStep)
	{
	case 0:// 飛び掛かり攻撃始め
		// 飛び掛かり攻撃
		if (ATTACKHAND_START_FRAME <= GetAnimationFrame())
		{
			// ドラゴンが向いている方向へ移動
			mMoveSpeed += VectorZ() * ATTACKHAND_MOVESPEED;
		}
		// ジャンプ処理
		if (JUMP_START_FRAME <= GetAnimationFrame())
		{
			mMoveSpeed += CVector(0.0f, 3.6f, 0.0f);
			mIsGrounded = false;
			mAttackStep++;
			AttackStart();
		}
		break;
	case 1:// 飛び掛かり攻撃
		if (ATTACKHAND_START_FRAME <= GetAnimationFrame())
		{
			// ドラゴンが向いている方向へ移動
			mMoveSpeed += VectorZ() * ATTACKHAND_MOVESPEED;
		}
		// 落下の調整
		if (FALLING_FRAME1_START <= GetAnimationFrame() &&
			GetAnimationFrame() <= FALLING_FRAME1_END)
		{
			mMoveSpeed += CVector(0.0f, -0.36f, 0.0f);
		}
		// 飛び掛かり攻撃の移動が終了したら
		if (ATTACKHAND_END_FRAME <= GetAnimationFrame())
		{
			mAttackStep++;
			AttackEnd();
		}
		break;
	case 2:// バックステップ
		// バックステップ移動処理
		if (BACKSTEP_START_FRAME <= GetAnimationFrame() &&
			GetAnimationFrame() <= BACKSTEP_END_FRAME)
		{
			// ドラゴンが向いている方向の逆方向へ移動
			mMoveSpeed += -VectorZ() * BACKSTEP_MOVESPEED;
		}
		// 落下の調整
		if (FALLING_FRAME2_START <= GetAnimationFrame() &&
			GetAnimationFrame() <= FALLING_FRAME2_END)
		{
			mMoveSpeed += CVector(0.0f, -0.33f, 0.0f);
		}
		// バックステップ(ジャンプ)
		if (BACKJUMP_START_FRAME <= GetAnimationFrame())
		{
			mMoveSpeed += CVector(0.0f, 2.5f, 0.0f);
			mIsGrounded = false;
			mAttackStep++;
		}
		break;
	case 3:// バックステップ
		// バックステップ移動処理
		if (BACKSTEP_START_FRAME <= GetAnimationFrame() &&
			GetAnimationFrame() <= BACKSTEP_END_FRAME)
		{
			// ドラゴンが向いている方向の逆方向へ移動
			mMoveSpeed += -VectorZ() * BACKSTEP_MOVESPEED;
		}
		// 落下の調整
		if (FALLING_FRAME2_START <= GetAnimationFrame() &&
			GetAnimationFrame() <= FALLING_FRAME2_END)
		{
			mMoveSpeed += CVector(0.0f, -0.4f, 0.0f);
		}
		break;
	}

	//地面に足がつかない不具合が発生するため
	//コライダーの位置を調整で対応
	if (18.0f <= GetAnimationFrame() && GetAnimationFrame() <= 25.0f)
	{
		mpColliderLine->Position(mpColliderLine->Position() + CVector(0.0f, 0.4f, 0.0f));
	}
	if (85.0f <= GetAnimationFrame())
	{
		mpColliderLine->Position(CVector::zero);
	}
}

// 地上ブレス攻撃処理
void CDragon::Update_AttackFlame()
{
	if (FLAME_START_FRAME <= GetAnimationFrame() &&
		GetAnimationFrame() <= FLAME_END_FRAME)
	{
		if (!mpFlamethrower->IsThrowing())
		{
			mpFlamethrower->Start();
		}
	}
	else if (mpFlamethrower->IsThrowing())
	{
		mpFlamethrower->Stop();
	}
}

// バックステップ
void CDragon::Update_BackStep()
{
	switch (mAttackStep)
	{
	case 0:
		// ジャンプ処理
		if (12.0f <= GetAnimationFrame())
		{
			mMoveSpeed += CVector(0.0f, 2.5f, 0.0f);
			mIsGrounded = false;
			mAttackStep++;
		}
		// 移動処理
		if (12.0f <= GetAnimationFrame() &&
			GetAnimationFrame() <= 21.0f)
		{
			mMoveSpeed += -VectorZ() * 4.5;
		}
		break;
	case 1:
		// 移動処理
		if (12.0f <= GetAnimationFrame() &&
			GetAnimationFrame() <= 32.0f)
		{
			mMoveSpeed += -VectorZ() * 4.5;
		}
		// 落下の調整
		if (22.0f <= GetAnimationFrame() &&
			GetAnimationFrame() <= 32.0f)
		{
			mMoveSpeed += CVector(0.0f, -0.3f, 0.0f);
		}
		break;
	}

	// コライダー処理
	if (GetAnimationFrame() >= 0.0f)
	{
		mpColliderLine->Position(0.0f, 13.0f, 0.0f);
	}
	if (GetAnimationFrame() >= 20.0f)
	{
		mpColliderLine->Position(CVector::zero);
	}
	if (IsAnimationFinished()) mAttackStep = 0;
}

// 残りのHPの割合を取得
int CDragon::GetHPPercent() const
{
	float hpper = Math::Clamp01((float)mStatus.hp / mMaxStatus.hp);
	int persent = hpper * 100;
	return persent;
}

// 戦闘アイドル時のランダム値生成
int CDragon::BattleIdleRand() const
{
	// 残りHPの割合を取得
	int persent = GetHPPercent();
	// 0～100の乱数を生成
	int rand = Math::Rand(0.0f, 100.0f);

	// 怒り状態かどうか
	if (!mIsAngry)
	{
		// 残りのHPの割合によって生成する乱数を調整
		// 20%以下
		if (persent <= 20)
		{
			if (rand <= 40) return 1;
			else if (rand <= 85) return 2;
			else return 3;
		}
		// 50%以下
		else if (persent <= 50)
		{
			if (rand <= 30) return 1;
			else if (rand <= 80) return 2;
			else return 3;
		}
		// 75%以下
		else if (persent <= 75)
		{
			if (rand <= 20) return 1;
			else if (rand <= 70) return 2;
			else return 3;
		}
		// 100%以下
		else
		{
			if (rand <= 30) return 1;
			else if (rand <= 80) return 2;
			else return 3;
		}
	}
	else // 怒り状態
	{
		// 残りのHPの割合によって生成する乱数を調整
		// 20%以下
		if (persent <= 20)
		{
			if (rand <= 35) return 1;
			else if (rand <= 80) return 2;
			else return 3;
		}
		// 50%以下
		else if (persent <= 50)
		{
			if (rand <= 25) return 1;
			else if (rand <= 75) return 2;
			else return 3;
		}
		// 100%以下
		else
		{
			if (rand <= 10) return 1;
			else if (rand <= 70) return 2;
			else return 3;
		}
	}
	return 0;
}