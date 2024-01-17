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

		// プレイヤーの場所まで移動する
		mMoveSpeed += EP * 0.5f;
	}
	else
	{
		mState = EState::eIdle;
		ChangeAnimation(EAnimType::eIdle);
	}
}