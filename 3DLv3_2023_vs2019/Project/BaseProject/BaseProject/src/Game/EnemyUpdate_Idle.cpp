#include "CEnemy.h"
#include "CPlayer.h"
#include "Time.h"
#include "Maths.h"

void CEnemy::Update_Idle()
{
	// 敵がプレイヤーを見つけた場合、戦闘状態に移行する
	if (IsFoundPlayer())
	{
		mState = EState::eMove;
		ChangeAnimation(EAnimType::eWalk);
		return;
	}

	if (IsAnimationFinished() && AnimationIndex() != (int)EAnimType::eIdle)
	{
		ChangeAnimation(EAnimType::eIdle);
	}

	// 20秒ごとにランダム値を生成
	mElapsedTime += Time::DeltaTime();
	if (mElapsedTime >= 20.0f)
	{
		int rand = Math::Rand(0.0f, 2.0f);
		printf("敵乱数 : %d\n", rand);
		// 経過時間を初期化
		mElapsedTime = 0.0f;

		// 生成した乱数に応じてアニメーションを変更
		switch (rand)
		{
		case 0:
			ChangeAnimation(EAnimType::eIdle);
		case 1:
			ChangeAnimation(EAnimType::eIdle2);
			break;
		case 2:
			ChangeAnimation(EAnimType::eIdle3);
			break;
		}
	}
}