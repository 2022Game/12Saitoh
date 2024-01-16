#include "CEnemy.h"
#include "CPlayer.h"
#include "Time.h"
#include "Maths.h"

void CEnemy::Update_Idle()
{
	if (IsAnimationFinished() && AnimationIndex() != (int)EAnimType::eIdle)
	{
		ChangeAnimation(EAnimType::eIdle);
	}

	// 20秒ごとにランダム値を生成
	mElapsedTime += Time::DeltaTime();
	if (mElapsedTime >= 20.0f)
	{
		int rand = Math::Rand(0.0f, 3.0f);
		printf("敵乱数 : %d\n", rand);
		// 経過時間を初期化
		mElapsedTime = 0.0f;

		// 生成した乱数に応じてアニメーションを変更
		switch (rand)
		{
		case 0:
			mState = EState::eMove;
			ChangeAnimation(EAnimType::eWalk);
			break;
		case 1:
			ChangeAnimation(EAnimType::eIdle2);
			break;
		case 2:
			ChangeAnimation(EAnimType::eIdle3);
			break;
		}
	}
}