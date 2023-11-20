#include "CPlayer.h"

void CPlayer::Update_AttackAngleVec(CVector *direction)
{
	mMoveSpeed.X(0.0f);
	mMoveSpeed.Z(0.0f);

	// キーの入力ベクトルを取得
	CVector input = CVector::zero;
	bool isinput = false;	// キーが入力されているか判定
	if (CInput::Key('W'))		input.Z(-1.0f);
	else if (CInput::Key('S'))	input.Z(1.0f);
	if (CInput::Key('A'))		input.X(-1.0f);
	else if (CInput::Key('D'))	input.X(1.0f);

	// 仮保存の入力ベクトルが初期値の場合
	if (mInput_save == CVector::zero)
	{
		// 入力ベクトルデータを一時的に保存
		mInput_save = input;
		isinput = true;
	}
	// 攻撃アニメーションが終了したら
	// 一時的に保存した入力ベクトルを初期化する
	if (IsAnimationFinished())
	{
		mInput_save = CVector::zero;
		isinput = false;
	}

	// プレイヤーの回転角度によってベクトルを入手
	CVector vec;
	CVector angle = EulerAngles();
	// 316 ～ 45度の間
	if (0 <= angle.Y() && angle.Y() <= 45 ||
		316 <= angle.Y() && angle.Y() <= 365)		vec.Z(1.0f);
	// 46 ～ 135度の間
	else if (46 <= angle.Y() && angle.Y() <= 135)	vec.X(-1.0f);
	// 136 ～ 225度の間
	else if (136 <= angle.Y() && angle.Y() <= 225)	vec.Z(-1.0f);
	// 226 ～ 315度の間
	else if (226 <= angle.Y() && angle.Y() <= 315)	vec.X(1.0f);

	// カメラの向きに合わせた移動ベクトルに変換
	// 移動キー入力があるかによって移動ベクトルを変更
	CVector anglevec = CVector::zero;
	if (isinput) {
		anglevec = CCamera::MainCamera()->Rotation() * vec;
	}
	else {
		anglevec = CCamera::MainCamera()->Rotation() * mInput_save;
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

	switch (mAttackStep)
	{
	case 0:
		if (IsAnimationFinished())
		{
			mAttackStep++;
			ChangeAnimation(EAnimType::eNormalWait1_1);
		}
		// 攻撃に合わせてプレイヤーを移動
		if (NORMALATTACK1_1_START_FRAME <= GetAnimationFrame() &&
			GetAnimationFrame() <= NORMALATTACK1_1_END_FRAME)
		{
			mMoveSpeed += anglevec * NORMALATTACK1_1_MOVESPEED;
		}
		break;
	case 1:
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
		if (CInput::Key('C') && CInput::Key(VK_LBUTTON))
		{
			// 弱闘技へ切り替え
			mAttackStep = 0;
			mState = EState::eSpecalMove;
			ChangeAnimation(EAnimType::eCounter_Start);
		}
		if (IsAnimationFinished())
		{
			mAttackStep = ATTACKSTEP_END;
			ChangeAnimation(EAnimType::eNormalEnd1_1);
		}
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
		if (IsAnimationFinished())
		{
			mAttackStep++;
			ChangeAnimation(EAnimType::eNormalWait1_2);
		}
		// 攻撃に合わせてプレイヤーを移動
		if (NORMALATTACK1_2_START_FRAME <= GetAnimationFrame() &&
			GetAnimationFrame() <= NORMALATTACK1_2_END_FRAME)
		{
			mMoveSpeed += anglevec * NORMALATTACK1_2_MOVESPEED;
		}
		break;
	case 1:
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
		if (CInput::Key('C') && CInput::Key(VK_LBUTTON))
		{
			// 弱闘技へ切り替え
			mAttackStep = 0;
			mState = EState::eSpecalMove;
			ChangeAnimation(EAnimType::eCounter_Start);
		}
		if (IsAnimationFinished())
		{
			mAttackStep = ATTACKSTEP_END;
			ChangeAnimation(EAnimType::eNormalEnd1_2);
		}
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
		if (IsAnimationFinished())
		{
			mAttackStep++;
			ChangeAnimation(EAnimType::eNormalWait1_3);
		}
		// 攻撃に合わせてプレイヤーを移動
		if (NORMALATTACK1_3_START_FRAME <= GetAnimationFrame() &&
			GetAnimationFrame() <= NORMALATTACK1_3_END_FRAME)
		{
			mMoveSpeed += anglevec * NORMALATTACK1_3_MOVESPEED;
		}
		break;
	case 1:
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
		if (CInput::Key('C') && CInput::Key(VK_LBUTTON))
		{
			// 弱闘技へ切り替え
			mAttackStep = 0;
			mState = EState::eSpecalMove;
			ChangeAnimation(EAnimType::eCounter_Start);
		}
		if (IsAnimationFinished())
		{
			mAttackStep = ATTACKSTEP_END;
			ChangeAnimation(EAnimType::eNormalEnd1_3);
		}
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
		if (IsAnimationFinished())
		{
			mAttackStep++;
			ChangeAnimation(EAnimType::eIdleAir_Combat);
			mMoveSpeed = CVector::zero;
		}
		// 攻撃に合わせてプレイヤーを移動
		if (ATTACK_UP_START_FRAME <= GetAnimationFrame() &&
			GetAnimationFrame() <= ATTACK_UP_END_FRAME)
		{
			mMoveSpeed += anglevec * ATTACK_UP_MOVE_SPEED;
		}
		// モーションに合わせてジャンプを行う
		if (ATTACK_UP_JUMPSTART_FRAME == GetAnimationFrame())
		{
			mMoveSpeed += CVector(0.0f, ATTACK_UP_JUMP_SPEED, 0.0f);
			mIsGrounded = false;
		}
		break;
	case 1:
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
		if (IsAnimationFinished())
		{
			mAttackStep++;
			ChangeAnimation(EAnimType::eAirAttackWait1_1);
		}
		// 攻撃中は落下しない
		mMoveSpeed += -mMoveSpeed;
		break;
	case 1:
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
		if (IsAnimationFinished())
		{
			mAttackStep++;
			ChangeAnimation(EAnimType::eAirAttackWait1_2);
		}
		// 攻撃中は落下しない
		mMoveSpeed += -mMoveSpeed;
		break;
	case 1:
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
		if (IsAnimationFinished())
		{
			mAttackStep++;
			ChangeAnimation(EAnimType::eAirAttackWait1_3);
		}
		// 攻撃中は落下しない
		mMoveSpeed += -mMoveSpeed;
		break;
	case 1:
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
	CVector anglevec;
	Update_AttackAngleVec(&anglevec);
	if (IsAnimationFinished())
	{
		mAttackStep = ATTACKSTEP_END;
		ChangeAnimation(EAnimType::eAirAttackEnd1_4);
	}
	// 攻撃中は落下しない
	mMoveSpeed += -mMoveSpeed;
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
		mState = EState::eIdle;
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
				mAttackStep = 0;
				mState = EState::eAvoidance;
				ChangeAnimation(EAnimType::eRollStart_Combat);
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
			mState = EState::eMove;
			ChangeAnimation(EAnimType::eRunStart_Combat);
			// 回避動作への切り替え
			if (CInput::PushKey(VK_SPACE))
			{
				mState = EState::eAvoidance;
				ChangeAnimation(EAnimType::eRollStart_Combat);
			}
		}
	}
	// 攻撃終了モーションが終了したら待機状態へ移行する
	if (IsAnimationFinished())
	{
		mAttackStep = 0;
		mState = EState::eIdle;
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