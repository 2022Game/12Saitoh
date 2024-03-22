#include "CDragon.h"
#include "Maths.h"
#include "Time.h"
#include "CColliderLine.h"
#include "CPlayer.h"

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
		}

		int index = AnimationIndex();
		if (index != (int)EDragonAnimType::eAttackHand &&
			index != (int)EDragonAnimType::eAttackFlame) 		
			mElapsedTime += Time::DeltaTime();

		// 5秒ごとに攻撃を行う
		if (mElapsedTime >= 5.0f)
		{
			// 経過時間を初期化
			mElapsedTime = 0.0f;

			// ひとまずランダム値を生成して、値に応じて攻撃を行う
			int rand = Math::Rand(0, 1);
			switch (rand)
			{
			case 0:
				ChangeAnimation(EDragonAnimType::eAttackMouth);
				break;
			case 1:
				ChangeAnimation(EDragonAnimType::eAttackHand);
				break;
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
	mMoveSpeed.X(0.0f);
	mMoveSpeed.Z(0.0f);

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