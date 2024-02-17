#include "CEnemy.h"

void CEnemy::AttackPattern1()
{
	switch (mAttackStep)
	{
	case 0: mAttackStep++; break;
	case 1:
		if (AnimationIndex() != (int)EAnimType::eAttack_Bite)
		{
			ChangeAnimation(EAnimType::eAttack_Bite);
		}
		// 噛みつき攻撃が終了したら次の攻撃へ移行する
		if (IsAnimationFinished())
		{
			ChangeAnimation(EAnimType::eAttack_Scratching);
			mAttackStep++;
		}
		break;
	case 2:
		// 攻撃が終了したら待機状態へ移行する
		if (IsAnimationFinished())
		{
			ChangeAnimation(EAnimType::eIdle);
			mState = EState::eIdle;
			mAttackStep = 0;
		}
		break;
	}
}

void CEnemy::Update_Attack()
{
	// 乱数を使って攻撃パターンを変更(予定)
	AttackPattern1();
}