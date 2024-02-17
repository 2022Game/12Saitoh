#include "CEnemy.h"
#include "CPlayer.h"

void CEnemy::Update_Move()
{
	mMoveSpeed.X(0.0f);
	mMoveSpeed.Z(0.0f);

	if (IsFoundPlayer())
	{
		CVector playerPos = CPlayer::Instance()->Position();
		CVector enemyPos = Position();
		// 自身からプレイヤーまでのベクトルを取得
		CVector EP = (playerPos - enemyPos).Normalized();
		// 自身からプレイヤーまでのベクトルの長さを取得
		float distance = (playerPos - enemyPos).Length();

		// プレイヤーの場所まで移動する
		mMoveSpeed += EP * 0.5f;

		// 攻撃範囲内に入ったら攻撃開始
		if (distance < STOP_LENGE)
		{
			// 移動を終了して攻撃に移行
			mState = EState::eAttack;
			mMoveSpeed = CVector::zero;
		}
	}
	else
	{
		mState = EState::eIdle;
		ChangeAnimation(EAnimType::eIdle);
	}
}