#include "CPlayer.h"

//　闘技処理
void CPlayer::Update_SpecialMove()
{
	// 弱闘技か強闘技か判定
	// false:弱闘技　true:強闘技
	bool isspmove = nullptr;

	// アニメーションによって闘技の弱闘技か強闘技か判定する
	switch (AnimationIndex())
	{
	// 弱闘技
	case (int)EAnimType::eCounter_Start:	isspmove = false; break;
	case (int)EAnimType::eCounter_Loop:		isspmove = false; break;
	case (int)EAnimType::eCounter_End:		isspmove = false; break;
	// 強闘技
	case (int)EAnimType::Num: isspmove = true; break;
	}

	// 判定した闘技の処理を実行
	if (isspmove) {
		// 強闘技
		Update_Strong_SpecalMove(); 
	}
	else {
		// 弱闘技
		Update_Weak_SpecalMove();
	}

	// 攻撃処理が終了
	if (mAttackStep == ATTACKSTEP_END)
	{
		Update_AttackEnd();
	}
}


// 弱闘技処理
void CPlayer::Update_Weak_SpecalMove()
{
	switch (mAttackStep)
	{
	case 0:// カウンター構え始め
		if (mIsCounter)
		{
			// カウンター攻撃を実行
			mAttackStep = ATTACKSTEP_COUNTER;
			ChangeAnimation(EAnimType::eParryAttack);
		}
		if (IsAnimationFinished())
		{
			mAttackStep++;
			ChangeAnimation(EAnimType::eCounter_Loop);
		}
		break;
	case 1:// カウンター構え中
		if (mIsCounter)
		{
			// カウンター攻撃を実行
			mAttackStep = ATTACKSTEP_COUNTER;
			ChangeAnimation(EAnimType::eParryAttack);
		}
		if (IsAnimationFinished())
		{
			mAttackStep = ATTACKSTEP_END;
			ChangeAnimation(EAnimType::eCounter_End);
		}
		break;
	case ATTACKSTEP_COUNTER:// カウンター攻撃
		// カウンター攻撃が終了
		if (IsAnimationFinished())
		{
			mIsCounter = false;
			mAttackStep = ATTACKSTEP_END;
			ChangeAnimation(EAnimType::eParryAttack_End);
		}
		break;
	}
}

// 強闘技処理
void CPlayer::Update_Strong_SpecalMove()
{

}
