#include "CDragon.h"
#include "Maths.h"
#include "Time.h"
#include "CColliderLine.h"
#include "CPlayer.h"
#include "CFlamethrower.h"

// 戦闘中の更新処理
void CDragon::UpdateBattle()
{
	// 怒り状態かどうか
	if (!mIsAngry)// 通常状態
	{
		// 攻撃が終わったらアイドルに戻す
		if (IsAnimationFinished())
		{
			ChangeAnimation(EDragonAnimType::eIdle1);
			mIdleFlag = true;
			// ブレス攻撃をしていたら、ブレスを止める
			if (mpFlamethrower->IsThrowing())mpFlamethrower->Stop();
		}

		// アイドル中なら経過時間を加算する
		if (mIdleFlag) mElapsedTime += Time::DeltaTime();

		// 5秒ごとに攻撃を行う
		if (mElapsedTime >= 5.0f)
		{
			// 経過時間を初期化
			mElapsedTime = 0.0f;
			mIdleFlag = false;
			mIsAttack = false;

			// プレイヤーとの距離を取得
			PlayerFromDistance();
		}

		// 攻撃中でなければ
		if (!mIsAttack)
		{
			// 距離に応じて処理を行う
			switch (mDistanceType)
			{
			case EDistanceType::eNear:// 近距離
			{
				CVector playerPos = CPlayer::Instance()->Position();
				CVector enemyPos = Position();
				// 自身からプレイヤーまでのベクトルの長さを取得
				float distance = (playerPos - enemyPos).Length();

				// 噛みつき攻撃の範囲内だった場合
				if (distance <= 120.0f)
				{
					// 噛みつき攻撃を行う
					ChangeAnimation(EDragonAnimType::eAttackMouth);
					mIsAttack = true;
				}
				else
				{
					// 自身からプレイヤーまでのベクトルを取得
					CVector EP = (playerPos - enemyPos).Normalized();
					// 噛みつき攻撃の範囲まで移動する
					mMoveSpeed += EP * 0.01f;
					ChangeAnimation(EDragonAnimType::eWalk);
				}
				break;
			}
			case EDistanceType::eMedium:// 中距離
				// ランダム値を生成して、行う攻撃を選択
				// 飛び掛かり攻撃が多めになるよう調整
				switch (Math::Rand(0, 1))
				{
				case 0:
				case 1:// ブレス攻撃
					ChangeAnimation(EDragonAnimType::eAttackFlame);
					mIsAttack = true;
					break;
				default:// 飛び掛かり攻撃
					ChangeAnimation(EDragonAnimType::eAttackHand);
					mIsAttack = true;
					break;
				}
				break;

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
					CVector playerPos = CPlayer::Instance()->Position();
					CVector enemyPos = Position();
					// 自身からプレイヤーまでのベクトルの長さを取得
					float distance = (playerPos - enemyPos).Length();

					// 移動
					if (distance >= 230.0f)
					{
						// 自身からプレイヤーまでのベクトルを取得
						CVector EP = (playerPos - enemyPos).Normalized();
						// 噛みつき攻撃の範囲まで移動する
						mMoveSpeed += EP * 0.1f;
						ChangeAnimation(EDragonAnimType::eWalk);
					}
					// 一定の範囲まで移動したら
					else
					{
						mRandSave = 0;
						// ランダム値を生成して、攻撃を決める
						int rand = Math::Rand(0, 1);
						// 近距離
						if (rand == 0) mDistanceType = EDistanceType::eNear;
						// 中距離
						else mDistanceType = EDistanceType::eMedium;
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
	}
	else // 怒り状態
	{
		// 一旦アイドル状態にする
		if (IsAnimationFinished())
		{
			ChangeAnimation(EDragonAnimType::eIdle1);
		}

		// 怒り状態になってからの時間を取得
		mAngryElapsedTime += Time::DeltaTime();

		// 怒り状態になってから1分経過で怒り状態を解除
		if (mAngryElapsedTime >= 60.0f)
		{
			mIsAngry = false;
			mAngryElapsedTime = 0.0f;
		}
	}

	// 攻撃処理の更新
	UpdateAttack();
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
		mMoveSpeed.X(0.0f);
		mMoveSpeed.Z(0.0f);
		if (GetAnimationFrame() == ATTACKMOUTH_COL_START) AttackStart();
		else if (GetAnimationFrame() == ATTACKMOUTH_COL_END) AttackEnd();
		break;

	case (int)EDragonAnimType::eAttackHand:// 飛び掛かり攻撃
		mMoveSpeed.X(0.0f);
		mMoveSpeed.Z(0.0f);
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
		mMoveSpeed.X(0.0f);
		mMoveSpeed.Z(0.0f);

		if (FLAME_START_FRAME <= GetAnimationFrame()&&
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
	default:// 攻撃中以外は、プレイヤーのいる方向へ回転(仮)

		CVector playerPos = CPlayer::Instance()->Position();
		CVector enemyPos = Position();
		// 自身からプレイヤーまでのベクトルを取得
		CVector EP = (playerPos - enemyPos).Normalized();
		
		// プレイヤーの方向へ向ける
		CVector current = VectorZ();
		CVector target = EP;
		target.Y(0.0f);

		CVector angle = CVector::Slerp(current, target, 0.02f);
		Rotation(CQuaternion::LookRotation(angle));
		break;
	}
}