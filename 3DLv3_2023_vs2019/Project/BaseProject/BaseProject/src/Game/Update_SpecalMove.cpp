#include "CPlayer.h"

// 闘技に必要な情報を設定
void CPlayer::SetSpecalMove(EAnimType type, bool cutin)
{
	mAttackStep = 0;
	ChangeState(EState::eSpecalMove);
	ChangeAnimation(type);

	// カットインが必要な場合
	// カットインカメラの設定を行う
	if (cutin)
	{
		mpCutIn_PowerAttack->Setup(this);
		mpCutIn_PowerAttack->Start();
	}
}

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
	case (int)EAnimType::ePowerAttack_Start:isspmove = true;  break;
	case(int)EAnimType::ePowerAttack_Loop:	isspmove = true;  break;
	case(int)EAnimType::ePowerAttack:		isspmove = true;  break;
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
	mMoveSpeed.X(0.0f);
	mMoveSpeed.Z(0.0f);

	switch (mAttackStep)
	{
	case 0:// カウンター構え始め
		if (mIsCounter)
		{
			// カウンター攻撃を実行
			mAttackStep = 2;
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
			mAttackStep = 2;
			ChangeAnimation(EAnimType::eParryAttack);
		}
		if (IsAnimationFinished())
		{
			mAttackStep = ATTACKSTEP_END;
			ChangeAnimation(EAnimType::eCounter_End);
		}
		break;
	case 2:
		// 武器に攻撃開始を使える
		mpSword->AttackStart();
		mAttackStep++;
		break;
	case 3:// カウンター攻撃
		// 2度目の攻撃時に武器に攻撃開始を伝える
		if (COUNTERATTACK_START <= GetAnimationFrame())
		{
			mpSword->AttackStart();
			mAttackStep++;
		}
		break;
	case 4:// カウンター攻撃
		// カウンター攻撃が終了
		if (IsAnimationFinished())
		{
			mIsCounter = false;
			mAttackStep = ATTACKSTEP_END;
			ChangeAnimation(EAnimType::eParryAttack_End);
			// 武器に攻撃終了を伝える
			mpSword->AttackEnd();
			// 無敵状態を解除
			InvincibleOFF();
		}
		break;
	}
}

// 強闘技処理
void CPlayer::Update_Strong_SpecalMove()
{
	mMoveSpeed.X(0.0f);
	mMoveSpeed.Z(0.0f);

	switch (mAttackStep)
	{
	case 0:// 強攻撃構え開始
		if (IsAnimationFinished())
		{
			mAttackStep++;
			ChangeAnimation(EAnimType::ePowerAttack_Loop);
		}
		break;
	case 1:// 強攻撃構え中
		if (IsAnimationFinished())
		{
			mAttackStep++;
			ChangeAnimation(EAnimType::ePowerAttack);
			Strong_SpecalMove_Move();
		}
		break;
	case 2:// 強闘技
		// 攻撃に合わせてプレイヤーを移動
		Strong_SpecalMove_Move();
		// 強闘技終了
		if (IsAnimationFinished())
		{
			// 武器に攻撃終了を伝える
			mpSword->AttackEnd();
			mAttackStep = ATTACKSTEP_END;
			mSPAttackStep = 0;
		}
		break;
	}
}

// 強闘技中のプレイヤーの移動処理
void CPlayer::Strong_SpecalMove_Move()
{
	switch (mSPAttackStep)
	{
	case 0:// コライダー有効化
		// 剣に攻撃開始を伝える
		mpSword->AttackStart();
		mSPAttackStep++;
		break;
	case 1:// 一度目の移動処理
		if (POWERATTACK1_START <= GetAnimationFrame() &&
			GetAnimationFrame() <= POWERATTACK1_END)
		{
			mMoveSpeed += POWERATTACK1_VEC * POWERATTACK_MOVESPEED;
		}
		// 移動が終了したら次のステップに移行
		else if (GetAnimationFrame() > POWERATTACK1_END)
		{
			// 剣に攻撃終了を伝える
			mpSword->AttackEnd();
			mSPAttackStep++;
		}
		break;
	case 2:// コライダー有効化
		// 剣に攻撃開始を伝える
		mpSword->AttackStart();
		mSPAttackStep++;
	case 3:// 二度目の移動処理
		if (POWERATTACK2_START <= GetAnimationFrame() &&
			GetAnimationFrame() <= POWERATTACK2_END)
		{
			mMoveSpeed += POWERATTACK2_VEC * POWERATTACK_MOVESPEED;
		}
		// 移動が終了したら次のステップに移行
		else if (GetAnimationFrame() > POWERATTACK2_END)
		{
			// 剣に攻撃終了を伝える
			mpSword->AttackEnd();
			mSPAttackStep++;
		}
		break;
	case 4:// コライダー有効化
		// 剣に攻撃開始を伝える
		mpSword->AttackStart();
		mSPAttackStep++;
		break;
	case 5:// 三度目の移動処理
		if (POWERATTACK3_START <= GetAnimationFrame() &&
			GetAnimationFrame() <= POWERATTACK3_END)
		{
			mMoveSpeed += POWERATTACK3_VEC * POWERATTACK_MOVESPEED;
		}
		// 移動が終了したら次のステップに移行
		else if (GetAnimationFrame() > POWERATTACK3_END)
		{
			// 剣に攻撃終了を伝える
			mpSword->AttackEnd();
			mSPAttackStep++;
		}
		break;
	case 6:// コライダー有効化
		// 剣に攻撃開始を伝える
		mpSword->AttackStart();
		mSPAttackStep++;
		break;
	case 7:// 四度目の移動処理
		if (POWERATTACK4_START <= GetAnimationFrame() &&
			GetAnimationFrame() <= POWERATTACK4_END)
		{
			mMoveSpeed += POWERATTACK4_VEC * POWERATTACK_MOVESPEED;
		}
		// 移動が終了したら次のステップに移行
		else if (GetAnimationFrame() > POWERATTACK4_END)
		{
			// 剣に攻撃終了を伝える
			mpSword->AttackEnd();
			mSPAttackStep++;
		}
		break;
	case 8:// コライダー有効化
		// 剣に攻撃開始を伝える
		mpSword->AttackStart();
		mSPAttackStep++;
		break;
	case 9:// 五度目の移動処理
		if (POWERATTACK5_START <= GetAnimationFrame() &&
			GetAnimationFrame() <= POWERATTACK5_END)
		{
			mMoveSpeed += POWERATTACK5_VEC * POWERATTACK_MOVESPEED;
		}
		// 移動が終了したら次のステップに移行
		else if (GetAnimationFrame() > POWERATTACK5_END)
		{
			// 剣に攻撃終了を伝える
			mpSword->AttackEnd();
			mSPAttackStep++;
		}
		break;
	case 10:// コライダー有効化
		// 剣に攻撃開始を伝える
		mpSword->AttackStart();
		mSPAttackStep++;
		break;
	case 11:// 六度目の移動処理
		if (POWERATTACK6_START <= GetAnimationFrame() &&
			GetAnimationFrame() <= POWERATTACK6_END)
		{
			mMoveSpeed += POWERATTACK6_VEC * POWERATTACK_MOVESPEED;
		}
		// 移動が終了したら次のステップに移行
		else if (GetAnimationFrame() >= POWERATTACK6_END)
		{
			// 剣に攻撃終了を伝える
			mpSword->AttackEnd();
			mSPAttackStep++;
		}
		break;
	case 12:// コライダー有効化
		// 剣に攻撃開始を伝える
		mpSword->AttackStart();
		mSPAttackStep++;
		break;
	case 13:// 七度目の移動処理
		if (POWERATTACK7_START <= GetAnimationFrame() &&
			GetAnimationFrame() <= POWERATTACK7_END)
		{
			mMoveSpeed -= POWERATTACL7_VEC;
		}
		if (GetAnimationFrame() <= POWERATTACK7_END) mpSword->AttackEnd();
		break;
	}
}