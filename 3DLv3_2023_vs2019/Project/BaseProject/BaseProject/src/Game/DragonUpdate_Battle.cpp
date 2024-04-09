#include "CDragon.h"
#include "Maths.h"
#include "Time.h"
#include "CColliderLine.h"
#include "CPlayer.h"
#include "CFlamethrower.h"

// 戦闘中の更新処理
void CDragon::UpdateBattle()
{
	mMoveSpeed.X(0.0f);
	mMoveSpeed.Z(0.0f);

	// 戦闘の段階によって処理を変更
	switch (mBatteleStep)
	{
	case 0:// 戦闘中のアイドル処理
		UpdateBattele_Idle();
		break;
	case 1:// 戦闘中の追跡処理
		UpdateBattle_Chase();
		break;
	case 2:// 攻撃処理
		UpdateAttack();
		break;
	}

	// 怒り状態かどうか
	if (mIsAngry)
	{
		// 怒り状態になってからの時間を取得
		mAngryElapsedTime += Time::DeltaTime();

		// 怒り状態になってから1分経過で怒り状態を解除
		if (mAngryElapsedTime >= 60.0f)
		{
			mIsAngry = false;
			mAngryElapsedTime = 0.0f;
		}
	}
}

// 戦闘中のアイドル処理
void CDragon::UpdateBattele_Idle()
{
	// 経過時間を加算する
	mElapsedTime += Time::DeltaTime();
	// 3秒ごとに攻撃を行う
	if (mElapsedTime >= 3.0f)
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
		break;

	case 2:// プレイヤーの元へ移動する
	{
		CVector playerPos = CPlayer::Instance()->Position();
		CVector enemyPos = Position();
		// 自身からプレイヤーまでのベクトルを取得
		CVector EP = (playerPos - enemyPos).Normalized();

		mMoveSpeed += EP * 0.3f;
		ChangeAnimation(EDragonAnimType::eWalk);
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
				mBatteleStep++;
				mChaseElapsedTime = 0.0f;
			}
			else
			{
				// 攻撃ができる範囲まで移動
				mMoveSpeed += EP * 0.6f;
				ChangeAnimation(EDragonAnimType::eWalk);
				// 移動時間を加算
				mChaseElapsedTime += Time::DeltaTime();
			}
			// 5秒以上移動した場合、中距離攻撃に移行する
			if (mChaseElapsedTime >= 5.0f)
			{
				mChaseElapsedTime = 0.0f;
				mDistanceType = EDistanceType::eMedium;
			}
			break;

		case 2:// バックステップを行い、その後攻撃に移行

			// バックステップ処理
			if (AnimationIndex() == (int)EDragonAnimType::eBackStep)
			{
				// ジャンプ処理
				if (GetAnimationFrame() == 33.0f)
				{
					mMoveSpeed += CVector(0.0f, 3.5f, 0.0f);
					mIsGrounded = false;
				}

				// 移動処理
				if (33.0f <= GetAnimationFrame() &&
					GetAnimationFrame() <= 65.0f)
				{
					mMoveSpeed += -VectorZ() * 4.0;
				}
				if (42.0f <= GetAnimationFrame() &&
					GetAnimationFrame() <= 60.0f)
				{
					mMoveSpeed += CVector(0.0f, -0.33f, 0.0f);
				}

				// コライダー処理
				if (GetAnimationFrame() >= 0.0f)
				{
					mpColliderLine->Position(0.0f, 13.0f, 0.0f);
				}
				if (GetAnimationFrame() >= 60.0f)
				{
					mpColliderLine->Position(CVector::zero);
				}
			}
			// バックステップが終了したら
			if (IsAnimationFinished())
			{
				// 乱数を生成して、攻撃を決定する
				int rand = Math::Rand(0, 1);
				if (rand == 0)
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

		default:// 最初に一度だけ実行
			// 噛みつき攻撃の範囲外だった場合
			if (distance >= 150.0f)
			{
				mRandSave = 1;
			}
			// 噛みつき範囲内だった場合
			else if ( 80.0f <= distance && distance <= 150.0f)
			{
				if (!(dotZ <= cosf(15 * M_PI / 180.0f)))
				{
					ChangeAnimation(EDragonAnimType::eAttackMouth);
					mBatteleStep++;
					mChaseElapsedTime = 0.0f;
				}
				else
				{
					mRandSave = 2;
					ChangeAnimation(EDragonAnimType::eBackStep);
				}
			}
			else// それ以外の場合
			{
				mRandSave = 2;
				ChangeAnimation(EDragonAnimType::eBackStep);
			}
			break;
		}
		break;
	}
	case EDistanceType::eMedium:// 中距離
	{
		// 自身からプレイヤーまでのベクトルを取得
		CVector EP = (playerPos - enemyPos).Normalized();
		CVector forward = VectorZ().Normalized();
		// 正面方向のベクトルとプレイヤーまでの
		// 内積から角度を求める
		float dotZ = forward.Dot(EP);

		// 求めた角度が視野角度外の場合、
		// プレイヤーのいる方向へ回転させる
		if (dotZ <= cosf(10 * M_PI / 180.0f))
		{
			mMoveSpeed += EP * 0.2f;
			ChangeAnimation(EDragonAnimType::eWalk);
		}
		else// 視野角度内
		{
			// ランダム値を生成して、行う攻撃を選択
			// 飛び掛かり攻撃が多めになるよう調整
			switch (Math::Rand(0, 5))
			{
			case 0:
			case 1:// ブレス攻撃
				ChangeAnimation(EDragonAnimType::eAttackFlame);
				mBatteleStep++;
				break;
			default:// 飛び掛かり攻撃
				ChangeAnimation(EDragonAnimType::eAttackHand);
				mBatteleStep++;
				break;
			}
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
				// 一定の範囲まで移動する
				mMoveSpeed += EP * 0.8f;
				ChangeAnimation(EDragonAnimType::eWalk);
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

			// 5秒以上移動したら、攻撃を行う
			if (mChaseElapsedTime >= 5.0f)
			{
				// 乱数を保存した変数を初期化
				mRandSave = 0;
				// ブレス攻撃を行う
				ChangeAnimation(EDragonAnimType::eAttackFlame);
				// 移動時間の初期化
				mChaseElapsedTime = 0.0f;
				mBatteleStep++;
			}
		}
		break;
		case 3:
			break;
		}
		break;
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
		// 咆哮用のコライダの切り替えなどを行う予定
		break;

	case (int)EDragonAnimType::eAttackMouth:// 噛みつき攻撃
		if (GetAnimationFrame() == ATTACKMOUTH_COL_START) AttackStart();
		else if (GetAnimationFrame() == ATTACKMOUTH_COL_END) AttackEnd();
		break;

	case (int)EDragonAnimType::eAttackHand:// 飛び掛かり攻撃
		// 移動処理
		// 飛び掛かり攻撃
		if (ATTACKHAND_START_FRAME <= GetAnimationFrame() &&
			GetAnimationFrame() <= ATTACKHAND_END_FRAME)
		{
			// ドラゴンが向いている方向へ移動
			mMoveSpeed += VectorZ() * ATTACKHAND_MOVESPEED;
		}
		// 飛び掛かり攻撃後のバックステップ
		else if (BACKSTEP_START_FRAME <= GetAnimationFrame() &&
			GetAnimationFrame() <= BACKSTEP_END_FRAME)
		{
			// ドラゴンが向いている方向の逆方向へ移動
			mMoveSpeed += -VectorZ() * BACKSTEP_MOVESPEED;
		}

		// ジャンプ処理
		if (GetAnimationFrame() == JUMP_START_FRAME)
		{
			mMoveSpeed += CVector(0.0f, 4.0f, 0.0f);
			mIsGrounded = false;
		}
		// 落下の調整
		else if (FALLING_FRAME1_START <= GetAnimationFrame() &&
			GetAnimationFrame() <= FALLING_FRAME1_END)
		{
			mMoveSpeed += CVector(0.0f, -0.4f, 0.0f);
		}
		// バックステップ
		else if (GetAnimationFrame() == BACKJUMP_START_FRAME)
		{
			mMoveSpeed += CVector(0.0f, 2.0f, 0.0f);
			mIsGrounded = false;
		}
		// 落下の調整
		else if (FALLING_FRAME2_START <= GetAnimationFrame() &&
			GetAnimationFrame() <= FALLING_FRAME2_END)
		{
			mMoveSpeed += CVector(0.0f, -0.33f, 0.0f);
		}

		// 地面に足がつかない不具合が発生するため
		// コライダーの位置を調整で対応
		if (36.0f <= GetAnimationFrame() && GetAnimationFrame() <= 60.0f)
		{
			mpColliderLine->Position(mpColliderLine->Position() + CVector(0.0f, 0.2f, 0.0f));
		}
		if (GetAnimationFrame() == 74.0f)
		{
			mpColliderLine->Position(0.0f, 4.0f, 0.0f);
		}
		else if (GetAnimationFrame() == 148.0f)
		{
			mpColliderLine->Position(CVector::zero);
		}
		break;

	case (int)EDragonAnimType::eAttackFlame:

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
		break;
	}

	// 攻撃が終了したらアイドルに移行する
	if (IsAnimationFinished())
	{
		mBatteleStep = 0;
		// ブレス攻撃をしていたら、ブレスを止める
		if (mpFlamethrower->IsThrowing())mpFlamethrower->Stop();
	}
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