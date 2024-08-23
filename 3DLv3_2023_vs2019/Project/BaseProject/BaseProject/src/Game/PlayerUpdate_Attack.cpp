#include "CPlayer.h"

// プレイヤーの向いている方向のベクトルを取得
void CPlayer::Update_AttackAngleVec(CVector *direction)
{
	mMoveSpeed.X(0.0f);
	mMoveSpeed.Z(0.0f);

	// キーの入力ベクトルを取得
	CVector input = CVector::zero;
	if (CInput::Key('W'))		input.Z(-1.0f);
	else if (CInput::Key('S'))	input.Z(1.0f);
	if (CInput::Key('A'))		input.X(-1.0f);
	else if (CInput::Key('D'))	input.X(1.0f);

	// 仮保存の入力ベクトルが初期値の場合
	if (mIsUpdateInput && input.LengthSqr() > 0.0f) 
	{
		// 入力ベクトルデータを一時的に保存
		mInput_save = mCamForward * input.Z() + mCamSide * input.X();
		mIsUpdateInput = false;
	}
	// 攻撃アニメーションが終了したら
	// 一時的に保存した入力ベクトルを初期化する
	if (IsAnimationFinished())
	{
		mIsUpdateInput = true;
	}

	// カメラの向きに合わせた移動ベクトルに変換
	// 移動キー入力があるかによって移動ベクトルを変更
	CVector anglevec = CVector::zero;
	if (mIsUpdateInput || mInput_save.LengthSqr() == 0.0f) {
		anglevec = VectorZ();
	}
	else {
		anglevec = mInput_save;
	}
	anglevec.Y(0.0f);
	anglevec.Normalize();

	*direction = anglevec;
}

// 通常攻撃1-1処理
void CPlayer::Update_NormalAttack1()
{
	CVector anglevec;
	Update_AttackAngleVec(&anglevec);
	CVector pos = Position();
	switch (mAttackStep)
	{
	case 0:
	{
		// 剣に攻撃開始を伝える
		mpSword->AttackStart();
		mAttackStep++;
		mpNormalAttackSE1->Play(0.2f);
	}
	case 1:
		if (IsAnimationFinished())
		{
			mAttackStep++;
			ChangeAnimation(EAnimType::eNormalWait1_1);
			// 剣に攻撃終了を伝える
			mpSword->AttackEnd();
		}
		// 攻撃に合わせてプレイヤーを移動
		if (NORMALATTACK1_1_START_FRAME <= GetAnimationFrame() &&
			GetAnimationFrame() <= NORMALATTACK1_1_END_FRAME)
		{
			mMoveSpeed += anglevec * NORMALATTACK1_1_MOVESPEED;
		}
		break;
	case 2:
		if (CInput::PushKey(VK_LBUTTON))
		{
			// 通常攻撃1-2へ切り替え
			mAttackStep = 0;
			ChangeAnimation(EAnimType::eNormalAttack1_2);
		}
		else if (CInput::PushKey(VK_MBUTTON))
		{
			// ジャンプ攻撃へ切り替え
			mAttackStep = 0;
			ChangeAnimation(EAnimType::eAttack_Up);
		}
		if (IsAnimationFinished())
		{
			mAttackStep = ATTACKSTEP_END;
			ChangeAnimation(EAnimType::eNormalEnd1_1);
		}

		// 闘技への切り替え入力が有れば闘技へ移行
		ChangeSpMove();
		Update_AttackWait();
		break;
	}
}

// 通常攻撃1-2処理
void CPlayer::Update_NormalAttack2()
{
	CVector anglevec;
	Update_AttackAngleVec(&anglevec);

	switch (mAttackStep)
	{
	case 0:
	{
		// 剣に攻撃開始を伝える
		mpSword->AttackStart();
		mAttackStep++;
		mpNormalAttackSE2->Play(0.2f);
	}
		break;
	case 1:
		if (IsAnimationFinished())
		{
			mAttackStep++;
			ChangeAnimation(EAnimType::eNormalWait1_2);
			// 剣に攻撃終了を伝える
			mpSword->AttackEnd();
		}
		// 攻撃に合わせてプレイヤーを移動
		if (NORMALATTACK1_2_START_FRAME <= GetAnimationFrame() &&
			GetAnimationFrame() <= NORMALATTACK1_2_END_FRAME)
		{
			mMoveSpeed += anglevec * NORMALATTACK1_2_MOVESPEED;
		}
		break;
	case 2:
		if (CInput::PushKey(VK_LBUTTON))
		{
			// 通常攻撃1-3へ切り替え
			mAttackStep = 0;
			ChangeAnimation(EAnimType::eNormalAttack1_3);
		}
		else if (CInput::PushKey(VK_MBUTTON))
		{
			// ジャンプ攻撃へ切り替え
			mAttackStep = 0;
			ChangeAnimation(EAnimType::eAttack_Up);
		}
		if (IsAnimationFinished())
		{
			mAttackStep = ATTACKSTEP_END;
			ChangeAnimation(EAnimType::eNormalEnd1_2);
		}
		// 闘技への切り替え入力が有れば闘技へ移行
		ChangeSpMove();
		Update_AttackWait();
		break;
	}
}

// 通常攻撃1-3処理
void CPlayer::Update_NormalAttack3()
{
	CVector anglevec;
	Update_AttackAngleVec(&anglevec);
	switch (mAttackStep)
	{
	case 0:	
	{
		// 剣に攻撃開始を伝える
		mpSword->AttackStart();
		mAttackStep++;
		mpNormalAttackSE1->Play(0.2f);
	}
		break;
	case 1:
		// 攻撃に合わせてプレイヤーを移動
		if (NORMALATTACK1_3_START_FRAME <= GetAnimationFrame() &&
			GetAnimationFrame() <= NORMALATTACK1_3_END_FRAME)
		{
			mMoveSpeed += anglevec * NORMALATTACK1_3_MOVESPEED;
		}
		if (GetAnimationFrame() >= NORMALATTACK1_3_END_FRAME)
		{
			mAttackStep++;
			mpSword->AttackEnd();
		}
		break;
	case 2:
	{
		if (NORMALATTACK1_3_COLLIDER <= GetAnimationFrame())
		{
			mpSword->AttackStart();
			mAttackStep++;
			mpNormalAttackSE2->Play(0.2f);
		}
	}
		break;
	case 3:
		if (IsAnimationFinished())
		{
			mAttackStep++;
			ChangeAnimation(EAnimType::eNormalWait1_3);
			// 剣に攻撃終了を伝える
			mpSword->AttackEnd();
		}
		break;
	case 4:
		if (CInput::PushKey(VK_LBUTTON))
		{
			// 通常攻撃1-1へ切り替え
			mAttackStep = 0;
			ChangeAnimation(EAnimType::eNormalAttack1_1);
		}
		else if (CInput::PushKey(VK_MBUTTON))
		{
			// ジャンプ攻撃へ切り替え
			mAttackStep = 0;
			ChangeAnimation(EAnimType::eAttack_Up);
		}
		if (IsAnimationFinished())
		{
			mAttackStep = ATTACKSTEP_END;
			ChangeAnimation(EAnimType::eNormalEnd1_3);
		}
		// 闘技への切り替え入力が有れば闘技へ移行
		ChangeSpMove();
		Update_AttackWait();
		break;
	}
}

// ジャンプ攻撃処理
void CPlayer::Update_AttackUp()
{
	CVector anglevec;
	Update_AttackAngleVec(&anglevec);
	switch (mAttackStep)
	{
	case 0:
	{
		// 剣に攻撃開始を伝える
		mpSword->AttackStart();
		mAttackStep++;
		CSound* jumpse = CResourceManager::Get<CSound>("JumpAttackSE");
		jumpse->Play(0.2f);
	}
		break;
	case 1:
		if (IsAnimationFinished())
		{
			mAttackStep++;
			ChangeAnimation(EAnimType::eIdleAir_Combat);
			mMoveSpeed = CVector::zero;
			// 剣に攻撃終了を伝える
			mpSword->AttackEnd();
		}
		// 攻撃に合わせてプレイヤーを移動
		if (ATTACK_UP_START_FRAME <= GetAnimationFrame() &&
			GetAnimationFrame() <= ATTACK_UP_END_FRAME)
		{
			mMoveSpeed += anglevec * ATTACK_UP_MOVE_SPEED;
		}
		// モーションに合わせてジャンプを行う
		if (mIsGrounded && 
			ATTACK_UP_JUMPSTART_FRAME <= GetAnimationFrame()){
			mMoveSpeed += CVector(0.0f, ATTACK_UP_JUMP_SPEED, 0.0f);
			mIsGrounded = false;
		}

		break;
	case 2:
		if (CInput::PushKey(VK_LBUTTON))
		{
			// 空中攻撃1-1へ切り替え
			mAttackStep = 0;
			ChangeAnimation(EAnimType::eAirAttack1_1);
			mIsAirAttack = true;
		}
		Update_AttackWait();
		break;
	}
}

// 空中攻撃1-1処理
void CPlayer::Update_AirAttack1()
{
	CVector anglevec;
	Update_AttackAngleVec(&anglevec);
	switch (mAttackStep)
	{
	case 0:
		// 剣に攻撃開始を伝える
		mpSword->AttackStart();
		mAttackStep++;
		mpNormalAttackSE1->Play(0.2f);
		break;
	case 1:
		if (IsAnimationFinished())
		{
			mAttackStep++;
			ChangeAnimation(EAnimType::eAirAttackWait1_1);
			// 剣に攻撃終了を伝える
			mpSword->AttackEnd();
		}
		// 攻撃中は落下しない
		mMoveSpeed += -mMoveSpeed;
		break;
	case 2:
		if (CInput::PushKey(VK_LBUTTON))
		{
			// 空中攻撃1-2へ切り替え
			mAttackStep = 0;
			ChangeAnimation(EAnimType::eAirAttack1_2);
		}
		if (IsAnimationFinished())
		{
			mAttackStep = ATTACKSTEP_END;
			ChangeAnimation(EAnimType::eAirAttackEnd1_1);
		}
		// 攻撃中は落下しない
		mMoveSpeed += CVector(0.0f, GRAVITY, 0.0f);
		break;
	}
}

// 空中攻撃1-2処理
void CPlayer::Update_AirAttack2()
{
	CVector anglevec;
	Update_AttackAngleVec(&anglevec);
	switch (mAttackStep)
	{
	case 0:
		// 剣に攻撃開始を伝える
		mpSword->AttackStart();
		mAttackStep++;
		mpNormalAttackSE1->Play(0.2f);
		break;
	case 1:
		if (IsAnimationFinished())
		{
			mAttackStep++;
			ChangeAnimation(EAnimType::eAirAttackWait1_2);
			// 剣に攻撃終了を伝える
			mpSword->AttackEnd();
		}
		// 攻撃中は落下しない
		mMoveSpeed += -mMoveSpeed;
		break;
	case 2:
		if (CInput::PushKey(VK_LBUTTON))
		{
			// 空中攻撃1-3へ切り替え
			mAttackStep = 0;
			ChangeAnimation(EAnimType::eAirAttack1_3);
		}
		if (IsAnimationFinished())
		{
			mAttackStep = ATTACKSTEP_END;
			ChangeAnimation(EAnimType::eAirAttackEnd1_2);
		}
		// 攻撃中は落下しない
		mMoveSpeed += CVector(0.0f, GRAVITY, 0.0f);
		break;
	}
}

// 空中攻撃1-3処理
void CPlayer::Update_AirAttack3()
{
	CVector anglevec;
	Update_AttackAngleVec(&anglevec);
	switch (mAttackStep)
	{
	case 0:
		// 剣に攻撃開始を伝える
		mpSword->AttackStart();
		mAttackStep++;
		mpNormalAttackSE1->Play(0.2f);
		break;
	case 1:
		if (IsAnimationFinished())
		{
			mAttackStep++;
			ChangeAnimation(EAnimType::eAirAttackWait1_3);
			// 剣に攻撃終了を伝える
			mpSword->AttackEnd();
		}
		// 攻撃中は落下しない
		mMoveSpeed += -mMoveSpeed;
		break;
	case 2:
		if (CInput::PushKey(VK_LBUTTON))
		{
			// 空中攻撃1-4に切り替え
			mAttackStep = 0;
			ChangeAnimation(EAnimType::eAirAttack1_4);
		}
		if (IsAnimationFinished())
		{
			mAttackStep = ATTACKSTEP_END;
			ChangeAnimation(EAnimType::eAirAttackEnd1_3);
		}
		mMoveSpeed += -mMoveSpeed;
		break;
	}
}

// 空中攻撃1-4処理
void CPlayer::Update_AirAttack4()
{
	switch (mAttackStep)
	{
	case 0:
		// 剣に攻撃開始を伝える
		mpSword->AttackStart();
		mAttackStep++;
		mpNormalAttackSE1->Play(0.2f);
		break;
	case 1:
		CVector anglevec;
		Update_AttackAngleVec(&anglevec);
		if (IsAnimationFinished())
		{
			mAttackStep = ATTACKSTEP_END;
			ChangeAnimation(EAnimType::eAirAttackEnd1_4);
			// 剣に攻撃終了を伝える
			mpSword->AttackEnd();
		}
		// 攻撃中は落下しない
		mMoveSpeed += -mMoveSpeed;
		break;
	}
}

// 攻撃待ち
// 共通の攻撃待ち時の処理
void CPlayer::Update_AttackWait()
{
	// 攻撃待ちモーションの間に何も入力がなければ、
	// アイドル状態に移行する
	if (IsAnimationFinished())
	{
		// 待機状態へ移行
		ChangeState(EState::eIdle);
		mAttackStep = 0;
	}

	if (mIsGrounded)
	{
		// 攻撃待ちモーション中に移動＋回避キーの入力があれば
		// 回避状態へ移行する
		if (CInput::Key('W') || CInput::Key('A') ||
			CInput::Key('S') || CInput::Key('D'))
		{
			if (CInput::PushKey(VK_SPACE))
			{
				if (mStatus.sp >= AVOID_SPCOST)
				{
					mAttackStep = 0;
					ChangeState(EState::eAvoidance);
					ChangeAnimation(EAnimType::eRollStart_Combat);
				}
			}
		}
	}
}

// 攻撃終了
void CPlayer::Update_AttackEnd()
{
	if (mIsGrounded)
	{
		// 移動キーが入力されたら移動状態へ移行する
		if (CInput::Key('W') || CInput::Key('A') ||
			CInput::Key('S') || CInput::Key('D'))
		{
			mAttackStep = 0;
			ChangeState(EState::eMove);
			ChangeState(EState::eMove);
			ChangeAnimation(EAnimType::eRunStart_Combat);
			// 回避動作への切り替え
			if (CInput::PushKey(VK_SPACE))
			{
				if (mStatus.sp >= AVOID_SPCOST)
				{
					ChangeState(EState::eAvoidance);
					ChangeAnimation(EAnimType::eRollStart_Combat);
				}
			}
		}
	}
	// 攻撃終了モーションが終了したら待機状態へ移行する
	if (IsAnimationFinished())
	{
		ChangeState(EState::eIdle);
		mAttackStep = 0;
	}
}

void CPlayer::Update_Attack()
{
	// アニメーションによって処理を変更
	switch (AnimationIndex())
	{
	case (int)EAnimType::eNormalAttack1_1:// 通常攻撃1-1処理
		Update_NormalAttack1();
		break;
	case (int)EAnimType::eNormalWait1_1:
		Update_NormalAttack1();
		break;
	case (int)EAnimType::eNormalAttack1_2:// 通常攻撃1-2処理
		Update_NormalAttack2();
		break;
	case (int)EAnimType::eNormalWait1_2:
		Update_NormalAttack2();
		break;
	case (int)EAnimType::eNormalAttack1_3:// 通常攻撃1-3処理
		Update_NormalAttack3();
		break;
	case (int)EAnimType::eNormalWait1_3:
		Update_NormalAttack3();
		break;
	case (int)EAnimType::eAttack_Up:// ジャンプ攻撃処理
		Update_AttackUp();
		break;
	case (int)EAnimType::eIdleAir_Combat:
		Update_AttackUp();
		break;
	case (int)EAnimType::eAirAttack1_1:// 空中攻撃1-1処理
		Update_AirAttack1();
		break;
	case (int)EAnimType::eAirAttackWait1_1:
		Update_AirAttack1();
		break;
	case (int)EAnimType::eAirAttack1_2:// 空中攻撃1-2処理
		Update_AirAttack2();
		break;
	case (int)EAnimType::eAirAttackWait1_2:
		Update_AirAttack2();
		break;
	case (int)EAnimType::eAirAttack1_3:// 空中攻撃1-3処理
		Update_AirAttack3();
		break;
	case (int)EAnimType::eAirAttackWait1_3:
		Update_AirAttack3();
		break;
	case (int)EAnimType::eAirAttack1_4:// 空中攻撃1-4
		Update_AirAttack4();
		break;
	}

	// 攻撃処理が終了
	if (mAttackStep == ATTACKSTEP_END)
	{
		// 攻撃終了処理を実行
		Update_AttackEnd();
	}
}

void CPlayer::ChangeSpMove()
{
	if (CInput::Key('C') && CInput::Key(VK_LBUTTON))
	{
		// 闘気が100以上あれば
		if (mStatus.touki >= 100)
		{
			// 弱闘技へ切り替え
			SetSpecalMove(EAnimType::eCounter_Start, false);
			mStatus.touki -= 100;
		}
	}
	if (CInput::Key('C') && CInput::Key(VK_RBUTTON))
	{
		// 闘気が200以上あれば
		if (mStatus.touki >= 200)
		{
			// 強闘技へ切り替え
			SetSpecalMove(EAnimType::ePowerAttack_Start, true);
			mStatus.touki -= 200;
		}
	}
}