//プレイヤークラスのインクルード
#include "CPlayer.h"
#include "CInput.h"
#include "CCamera.h"
#include "CDebugPrint.h"
#include "PlayerData.h"

// プレイヤーのインスタンス
CPlayer* CPlayer::spInstance = nullptr;

// コンストラクタ
CPlayer::CPlayer()
	: CXCharacter(ETag::ePlayer, ETaskPriority::ePlayer)
	, mState(EState::eIdle)
	, mInput_save(CVector::zero)
	, mIsGrounded(false)
	, mIsDrawn(false)
	, mIsAirAttack(false)
	, mpRideObject(nullptr)
{
	//インスタンスの設定
	spInstance = this;

	// モデルデータ読み込み
	CModelX* model = new CModelX();
	model->Load(MODEL_PATH);

	// テーブル内のアニメーションデータを読み込み
	int size = ARRAY_SIZE(PlayerData::ANIM_DATA);
	for (int i = 0; i < size; i++)
	{
		const PlayerData::AnimData& data = PlayerData::ANIM_DATA[i];
		if (data.path.empty()) continue;
		model->AddAnimationSet(data.path.c_str());
	}
	// CXCharacterの初期化
	Init(model);

	// 最初は待機アニメーションを再生
	ChangeAnimation(EAnimType::eIdle);

	//線分コライダの設定
	mpColliderLine = new CColliderLine
	(
		this, ELayer::eField,
		CVector(0.0f, 0.0f, 0.0f),
		CVector(0.0f, PLAYER_HEIGHT, 0.0f)
	);
	mpColliderLine->SetCollisionLayers({ ELayer::eField });
}

CPlayer::~CPlayer()
{
	SAFE_DELETE(mpColliderLine);
	SAFE_DELETE(mpModel);
	SAFE_DELETE(mpSword);
}

CPlayer* CPlayer::Instance()
{
	return spInstance;
}

// アニメーション切り替え
void CPlayer::ChangeAnimation(EAnimType type)
{
	if (!(EAnimType::None < type && type < EAnimType::Num)) return;
	PlayerData::AnimData data = PlayerData::ANIM_DATA[(int)type];
	CXCharacter::ChangeAnimation((int)type, data.loop, data.frameLength);
}

// 抜納状態を切り替える
void CPlayer::SwitchDrawn()
{
	// 抜刀状態の時、納刀状態へ切り替える
	if (mIsDrawn){mIsDrawn = false;}
	// 納刀状態の時、抜刀状態へ切り替える
	else{mIsDrawn = true;}
}

// 抜納の切り替え処理
void CPlayer::Update_SwitchDrawn()
{
	// 特定のアニメーションで抜納状態を切り替える
	// アニメーション中のフレームによって切り替えるタイミングを決める
	switch (mIsDrawn)
	{
	case true:	// 抜刀
		switch (mState)	// プレイヤーの状態
		{
		case CPlayer::EState::eIdle: // 待機状態
			if (AnimationIndex() == (int)EAnimType::eIdle_Sheathed_Combat &&
				GetAnimationFrame() == SWITCH_SHEATHED_IDLE_FRAME){
				SwitchDrawn();
			}
			break;
		case CPlayer::EState::eMove: // 移動状態
			if (AnimationIndex() == (int)EAnimType::eRun_Sheathed_Combat &&
				GetAnimationFrame() == SWITCH_SHEATHED_RUN_FRAME){
				SwitchDrawn();
			}
		}
		break;
	case false:	// 納刀
		switch (mState) // プレイヤーの状態
		{
		case CPlayer::EState::eIdle: // 待機状態
			if (AnimationIndex() == (int)EAnimType::eIdle_Drawn_Combat &&
				GetAnimationFrame() == SWITCH_DRAWN_IDLE_FRAME) {
				SwitchDrawn();
			}
			break;
		case CPlayer::EState::eMove: // 移動状態
			if (AnimationIndex() == (int)EAnimType::eRun_Drawn_Combat) {
				if (GetAnimationFrame() == SWITCH_DRAWN_RUN_FRAME)
				SwitchDrawn();
			}
		}
		break;
	}
}

// 待機
void CPlayer::Update_Idle()
{
	mMoveSpeed.X(0.0f);
	mMoveSpeed.Z(0.0f);
	
	// 抜納状態の判定
	switch (mIsDrawn)
	{
	case true:	// 抜刀時
		// 地面に接地しているか判定
		if (mIsGrounded)
		{
			// 移動キーが押されたか判定
			if (CInput::Key('W') || CInput::Key('A') || CInput::Key('S') || CInput::Key('D'))
			{
				// 走り出しのアニメーションを再生
				ChangeAnimation(EAnimType::eRunStart_Combat);
				mState = EState::eMove;
			}
			else
			{
				// アニメーションが終了したら
				// アイドルアニメーションに切り替える
				if (IsAnimationFinished())
				{
					ChangeAnimation(EAnimType::eIdle_Combat);
				}
			}
			// 左クリックで通常攻撃へ移行
			if (CInput::PushKey(VK_LBUTTON))
			{
				mState = EState::eAttack;
				ChangeAnimation(EAnimType::eNormalAttack1_1);
			}
			// マウスホイールクリックでジャンプ攻撃へ移行
			if (CInput::PushKey(VK_MBUTTON))
			{
				mState = EState::eAttack;
				ChangeAnimation(EAnimType::eAttack_Up);
			}
			// Eキーで納刀
			if (CInput::PushKey('E'))
			{
				ChangeAnimation(EAnimType::eIdle_Sheathed_Combat);
			}
		}
		else
		{
			ChangeAnimation(EAnimType::eIdleAir_Combat);
			// 一度も空中攻撃を行っていない場合、
			// 左クリックで空中攻撃へ移行する
			if (!mIsAirAttack && CInput::PushKey(VK_LBUTTON))
			{
				mState = EState::eAttack;
				ChangeAnimation(EAnimType::eAirAttack1_1);
				mIsAirAttack = true;
			}
		}
		break;
	case false:	// 納刀時
		// 地面に接地しているか判定
		if (mIsGrounded)
		{
			// 移動キーが押されたか判定
			if (CInput::Key('W') || CInput::Key('A') || CInput::Key('S') || CInput::Key('D'))
			{
				// 走り出しのアニメーションを再生
				ChangeAnimation(EAnimType::eRunStart);
				mState = EState::eMove;
				// ダッシュキーが押されている場合ダッシュ移動状態に切り替える
				if (CInput::Key(VK_SHIFT))
				{
					mState = EState::eFastMove;
					ChangeAnimation(EAnimType::eFastRunStart);
				}
			}
			else // 移動キーが押されていない時
			{
				// アニメーションが終了したら
				// アイドルアニメーションに切り替える
				if (IsAnimationFinished())
				{
					ChangeAnimation(EAnimType::eIdle);
				}
				// 左クリックまたは、マウスホイールクリックで抜刀
				if (CInput::PushKey(VK_LBUTTON) || CInput::PushKey(VK_MBUTTON))
				{
					// 抜刀アニメーションを再生
					ChangeAnimation(EAnimType::eIdle_Drawn_Combat);
				}
			}
		}
		break;
	}
}

//移動
void CPlayer::Update_Move()
{
	mMoveSpeed.X(0.0f);
	mMoveSpeed.Z(0.0f);

	//抜納状態の判定
	switch (mIsDrawn)
	{
	case true:	// 抜刀
		// 地面に接地しているか判定
		if (mIsGrounded)
		{
			// 移動処理
			// キーの入力ベクトルを取得
			CVector input;
			if (CInput::Key('W'))		input.Z(-1.0f);
			else if (CInput::Key('S'))	input.Z(1.0f);
			if (CInput::Key('A'))		input.X(-1.0f);
			else if (CInput::Key('D'))	input.X(1.0f);

			// 入力ベクトルの長さで入力されているか判定
			if (input.LengthSqr() > 0)
			{
				// カメラの向きに合わせた移動ベクトルに変換
				CVector move = CCamera::MainCamera()->Rotation() * input;
				move.Y(0.0f);
				move.Normalize();

				mMoveSpeed += move * MOVE_SPEED;

				// アニメーションが終了
				if (IsAnimationFinished())
				{
					// 走りアニメーションに切り替え
					ChangeAnimation(EAnimType::eRun_Combat);
				}
				// 回避動作への切り替え
				if (CInput::PushKey(VK_SPACE))
				{
					mState = EState::eAvoidance;
					ChangeAnimation(EAnimType::eRollStart_Combat);
				}
				// 左クリックで攻撃状態へ移行
				if (CInput::PushKey(VK_LBUTTON))
				{
					mState = EState::eAttack;
					ChangeAnimation(EAnimType::eNormalAttack1_1);
				}
				// マウスホイールクリックでジャンプ攻撃へ移行
				if (CInput::PushKey(VK_MBUTTON))
				{
					mState = EState::eAttack;
					ChangeAnimation(EAnimType::eAttack_Up);
				}
				// Eキーで納刀
				if (CInput::PushKey('E'))
				{
					ChangeAnimation(EAnimType::eRun_Sheathed_Combat);
				}
			}
			// 移動キーが押されていない
			else
			{
				// 走り終わりのアニメーションを再生し、アイドル状態に戻す
				ChangeAnimation(EAnimType::eRunEnd_Combat);
				mState = EState::eIdle;
			}
		}
		else
		{
			ChangeAnimation(EAnimType::eIdleAir_Combat);
			mState = EState::eIdle;
		}
		break;
	case false:	// 納刀
		// 地面に接地しているか判定
		if (mIsGrounded)
		{
			// 移動処理
			// キーの入力ベクトルを取得
			CVector input;
			if (CInput::Key('W'))		input.Z(-1.0f);
			else if (CInput::Key('S'))	input.Z(1.0f);
			if (CInput::Key('A'))		input.X(-1.0f);
			else if (CInput::Key('D'))	input.X(1.0f);

			// 入力ベクトルの長さで入力されているか判定
			if (input.LengthSqr() > 0)
			{
				// カメラの向きに合わせた移動ベクトルに変換
				CVector move = CCamera::MainCamera()->Rotation() * input;
				move.Y(0.0f);
				move.Normalize();

				mMoveSpeed += move * MOVE_SPEED;

				// アニメーションが終了
				if (IsAnimationFinished())
				{
					// 走りアニメーションに切り替え
					ChangeAnimation(EAnimType::eRun);
				}
				// ダッシュ移動の切り替え
				if (CInput::Key(VK_SHIFT))
				{
					mState = EState::eFastMove;
					ChangeAnimation(EAnimType::eFastRun);
				}
				// 回避動作への切り替え
				if (CInput::PushKey(VK_SPACE))
				{
					mState = EState::eAvoidance;
					ChangeAnimation(EAnimType::eRollStart);
				}
				// 左クリックまたは、マウスホイールクリックで抜刀
				if (CInput::PushKey(VK_LBUTTON) || CInput::PushKey(VK_MBUTTON))
				{
					ChangeAnimation(EAnimType::eRun_Drawn_Combat);
				}
			}
			// 移動キーが押されていない
			else
			{
				// 走り終わりのアニメーションを再生し、アイドル状態に戻す
				ChangeAnimation(EAnimType::eRunEnd);
				mState = EState::eIdle;
			}
		}
		else
		{
			ChangeAnimation(EAnimType::eIdleAir);
			mState = EState::eIdle;
		}
		break;
	}
}

//ダッシュ移動
void CPlayer::Update_FastMove()
{
	mMoveSpeed.X(0.0f);
	mMoveSpeed.Z(0.0f);

	//地面に接地しているか判定
	if (mIsGrounded)
	{
		// 移動処理
		// キーの入力ベクトルを取得
		CVector input;
		// ダッシュキーの入力判定	true:入力中 false:非入力中
		bool dash = true;
		if (CInput::Key('W'))		input.Z(-1.0f);
		else if (CInput::Key('S'))	input.Z(1.0f);
		if (CInput::Key('A'))		input.X(-1.0f);
		else if (CInput::Key('D'))	input.X(1.0f);
		if (CInput::PullKey(VK_SHIFT)) dash = false;

		// 入力ベクトルの長さで入力されているか判定
		if (input.LengthSqr() > 0)
		{
			// カメラの向きに合わせた移動ベクトルに変換
			CVector move = CCamera::MainCamera()->Rotation() * input;
			move.Y(0.0f);
			move.Normalize();

			mMoveSpeed += move * FASTMOVE_SPEED;

			// ダッシュ開始アニメーションが終了
			if (IsAnimationFinished())
			{
				// ダッシュアニメーションに切り替え
				ChangeAnimation(EAnimType::eFastRun);
			}
			// 回避動作への切り替え
			if (CInput::PushKey(VK_SPACE))
			{
				mState = EState::eAvoidance;
				ChangeAnimation(EAnimType::eRollStart);
			}
			// ダッシュキーを離した場合
			// ダッシュのアニメーションから走るアニメーションに切り替える
			if (dash == false)
			{
				mState = EState::eMove;
				ChangeAnimation(EAnimType::eRun);
			}
		}
		// 移動キーが押されていない
		else
		{
			// 走り終わりのアニメーションを再生し、アイドル状態に戻す
			ChangeAnimation(EAnimType::eFastRunEnd);
			mState = EState::eIdle;
		}
	}
	else
	{
		ChangeAnimation(EAnimType::eIdleAir);
		mState = EState::eIdle;
	}
}

// 回避動作
void CPlayer::Update_Avoidance()
{
	mMoveSpeed.X(0.0f);
	mMoveSpeed.Z(0.0f);

	// キーの入力ベクトルを取得
	CVector input;

	if (CInput::Key('W'))		input.Z(-1.0f);
	else if (CInput::Key('S'))	input.Z(1.0f);
	if (CInput::Key('A'))		input.X(-1.0f);
	else if (CInput::Key('D'))	input.X(1.0f);

	// 仮保存の入力ベクトルが初期値の場合
	if (mInput_save == CVector::zero)
	{
		// 入力ベクトルデータを一時的に保存
		mInput_save = input;
	}
	// カメラの向きに合わせた移動ベクトルに変換
	CVector move = CCamera::MainCamera()->Rotation() * mInput_save;
	move.Y(0.0f);
	move.Normalize();

	mMoveSpeed += move * ROLL_SPEED;

	// 回避動作が終了したら、アイドル状態へ移行する
	if (IsAnimationFinished())
	{
		// 回避動作後に移動キー入力があれば走り状態へ移行
		if (input.LengthSqr() > 0)
		{
			mState = EState::eMove;
			// 納抜の判定
			if (mIsDrawn)// 抜刀
			{
				ChangeAnimation(EAnimType::eRollEnd_run_Combat); 
			}
			else// 納刀
			{
				ChangeAnimation(EAnimType::eRollEnd_run); 
				// ダッシュキーが押されている場合、ダッシュに切り替える(納刀中のみ)
				if (CInput::Key(VK_SHIFT))
				{
					mState = EState::eFastMove;
				}
			}
		}
		// キー入力がない場合はアイドル状態へ移行
		else
		{
			mState = EState::eIdle;
			// 納抜の判定
			if (mIsDrawn)// 抜刀
			{
				ChangeAnimation(EAnimType::eRollEnd_idle_Combat);
			}
			else// 納刀
			{
				ChangeAnimation(EAnimType::eRollEnd_idle);
			}
		}
		// 一時的に保存した入力ベクトルを初期値に戻す
		mInput_save = CVector::zero;
	}
}

// 攻撃
void CPlayer::Update_Attack()
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
	// 316 〜 45度の間
	if (0 <= angle.Y() && angle.Y() <= 45 ||
		316 <= angle.Y() && angle.Y() <= 365)		vec.Z(1.0f);
	// 46 〜 135度の間
	else if (46 <= angle.Y() && angle.Y() <= 135)	vec.X(-1.0f);
	// 136 〜 225度の間
	else if (136 <= angle.Y() && angle.Y() <= 225)	vec.Z(-1.0f);
	// 226 〜 315度の間
	else if (226 <= angle.Y() && angle.Y() <= 315)	vec.X(1.0f);

	// カメラの向きに合わせた移動ベクトルに変換
	// 移動キー入力があるかによって移動ベクトルを変更dd
	CVector anglevec = CVector::zero;
	if (isinput) {
		anglevec = CCamera::MainCamera()->Rotation() * vec;
	}
	else {
		anglevec = CCamera::MainCamera()->Rotation() * mInput_save;
	}
	anglevec.Y(0.0f);
	anglevec.Normalize();

	switch (AnimationIndex())
	{
	case (int)EAnimType::eNormalAttack1_1:// 通常攻撃1-1処理
		if (IsAnimationFinished())
		{
			mState = EState::eAttackWait;
			ChangeAnimation(EAnimType::eNormalWait1_1);
		}
		// 攻撃に合わせてプレイヤーを移動
		if (NORMALATTACK1_1_START_FRAME <= GetAnimationFrame() &&
			GetAnimationFrame() <= NORMALATTACK1_1_END_FRAME)
		{
			mMoveSpeed += anglevec * NORMALATTACK1_1_MOVESPEED;
		}
		break;
	case (int)EAnimType::eNormalAttack1_2:// 通常攻撃1-2処理
		if (IsAnimationFinished())
		{
			mState = EState::eAttackWait;
			ChangeAnimation(EAnimType::eNormalWait1_2);
		}
		// 攻撃に合わせてプレイヤーを移動
		if (NORMALATTACK1_2_START_FRAME <= GetAnimationFrame() &&
			GetAnimationFrame() <= NORMALATTACK1_2_END_FRAME)
		{
			mMoveSpeed += anglevec * NORMALATTACK1_2_MOVESPEED;
		}
		break;
	case (int)EAnimType::eNormalAttack1_3:// 通常攻撃1-3処理
		if (IsAnimationFinished())
		{
			mState = EState::eAttackWait;
			ChangeAnimation(EAnimType::eNormalWait1_3);
		}
		// 攻撃に合わせてプレイヤーを移動
		if (NORMALATTACK1_3_START_FRAME <= GetAnimationFrame() &&
			GetAnimationFrame() <= NORMALATTACK1_3_END_FRAME)
		{
			mMoveSpeed += anglevec * NORMALATTACK1_3_MOVESPEED;
		}
		break;
	case (int)EAnimType::eAttack_Up:// ジャンプ攻撃処理
		if (IsAnimationFinished())
		{
			mState = EState::eAttackWait;
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
	case (int)EAnimType::eAirAttack1_1:// 空中攻撃1-1処理
		if (IsAnimationFinished())
		{
			mState = EState::eAttackWait;
			ChangeAnimation(EAnimType::eAirAttackWait1_1);
		}
		// 攻撃中は落下しない
		mMoveSpeed += -mMoveSpeed;
		break;
	case (int)EAnimType::eAirAttack1_2:// 空中攻撃1-2処理
		if (IsAnimationFinished())
		{
			mState = EState::eAttackWait;
			ChangeAnimation(EAnimType::eAirAttackWait1_2);
		}
		// 攻撃中は落下しない
		mMoveSpeed += -mMoveSpeed;
		break;
	case (int)EAnimType::eAirAttack1_3:// 空中攻撃1-3処理
		if (IsAnimationFinished())
		{
			mState = EState::eAttackWait;
			ChangeAnimation(EAnimType::eAirAttackWait1_3);
		}
		// 攻撃中は落下しない
		mMoveSpeed += -mMoveSpeed;
		break;
	case (int)EAnimType::eAirAttack1_4:// 空中攻撃1-4処理
		if (IsAnimationFinished())
		{
			mState = EState::eAttackEnd;
			ChangeAnimation(EAnimType::eAirAttackEnd1_4);
		}
		// 攻撃中は落下しない
		mMoveSpeed += -mMoveSpeed;
		break;
	}

}

// 攻撃終了待ち
void CPlayer::Update_AttackWait()
{
	switch (AnimationIndex())
	{
	case (int)EAnimType::eNormalWait1_1:// 通常攻撃1-1
		if (CInput::PushKey(VK_LBUTTON))
		{
			// 通常攻撃1-2へ切り替え
			mState = EState::eAttack;
			ChangeAnimation(EAnimType::eNormalAttack1_2);
		}
		else if (CInput::PushKey(VK_MBUTTON))
		{
			// ジャンプ攻撃へ切り替え
			mState = EState::eAttack;
			ChangeAnimation(EAnimType::eAttack_Up);
		}
		if (IsAnimationFinished())
		{
			mState = EState::eAttackEnd;
			ChangeAnimation(EAnimType::eNormalEnd1_1);
		}
		break;
	case (int)EAnimType::eNormalWait1_2:// 通常攻撃1-2
		if (CInput::PushKey(VK_LBUTTON))
		{
			// 通常攻撃1-3へ切り替え
			mState = EState::eAttack;
			ChangeAnimation(EAnimType::eNormalAttack1_3);
		}
		else if (CInput::PushKey(VK_MBUTTON))
		{
			// ジャンプ攻撃へ切り替え
			mState = EState::eAttack;
			ChangeAnimation(EAnimType::eAttack_Up);
		}
		if (IsAnimationFinished())
		{
			mState = EState::eAttackEnd;
			ChangeAnimation(EAnimType::eNormalEnd1_2);
		}
		break;
	case (int)EAnimType::eNormalWait1_3:// 通常攻撃1-3
		if (CInput::PushKey(VK_LBUTTON))
		{
			// 通常攻撃1-1へ切り替え
			mState = EState::eAttack;
			ChangeAnimation(EAnimType::eNormalAttack1_1);
		}
		else if (CInput::PushKey(VK_MBUTTON))
		{
			// ジャンプ攻撃へ切り替え
			mState = EState::eAttack;
			ChangeAnimation(EAnimType::eAttack_Up);
		}
		if (IsAnimationFinished())
		{
			mState = EState::eAttackEnd;
			ChangeAnimation(EAnimType::eNormalEnd1_3);
		}
		break;
	case (int)EAnimType::eIdleAir_Combat:// ジャンプ攻撃
		if (CInput::PushKey(VK_LBUTTON))
		{
			// 空中攻撃1-1へ切り替え
			mState = EState::eAttack; 
			ChangeAnimation(EAnimType::eAirAttack1_1);
			mIsAirAttack = true;
		}
		break;
	case (int)EAnimType::eAirAttackWait1_1:// 空中攻撃1-1
		if (CInput::PushKey(VK_LBUTTON))
		{
			// 空中攻撃1-2へ切り替え
			mState = EState::eAttack;
			ChangeAnimation(EAnimType::eAirAttack1_2);
		}
		if (IsAnimationFinished())
		{
			mState = EState::eAttackEnd;
			ChangeAnimation(EAnimType::eAirAttackEnd1_1);
		}
		// 攻撃中は落下しない
		mMoveSpeed += CVector(0.0f, GRAVITY, 0.0f);
		break;
	case (int)EAnimType::eAirAttackWait1_2:// 空中攻撃1-2
		if (CInput::PushKey(VK_LBUTTON))
		{
			// 空中攻撃1-3へ切り替え
			mState = EState::eAttack;
			ChangeAnimation(EAnimType::eAirAttack1_3);
		}
		if (IsAnimationFinished())
		{
			mState = EState::eAttackEnd;
			ChangeAnimation(EAnimType::eAirAttackEnd1_2);
		}
		// 攻撃中は落下しない
		mMoveSpeed += CVector(0.0f, GRAVITY, 0.0f);
		break;
	case (int)EAnimType::eAirAttackWait1_3:
		if (CInput::PushKey(VK_LBUTTON))
		{
			// 空中攻撃1-4に切り替え
			mState = EState::eAttack;
			ChangeAnimation(EAnimType::eAirAttack1_4);
		}
		if (IsAnimationFinished())
		{
			mState = EState::eAttackEnd;
			ChangeAnimation(EAnimType::eAirAttackEnd1_3);
		}
		mMoveSpeed += -mMoveSpeed;
		break;
	}
	// 攻撃待ちモーションの間に何も入力がなければ、
	// アイドル状態に移行する
	if (IsAnimationFinished())
	{
		// 待機状態へ移行
		mState = EState::eIdle;
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
		mState = EState::eIdle;
	}
}

// 更新
void CPlayer::Update()
{
	SetParent(mpRideObject);
	mpRideObject = nullptr;

	// 状態に合わせて、更新処理を切り替える
	switch (mState)
	{
		// 待機状態
		case EState::eIdle:
			Update_Idle();
			break;
		// 移動状態
		case EState::eMove:
			Update_Move();
			break;
		// ダッシュ移動
		case EState::eFastMove:
			Update_FastMove();
			break;
		// 回避状態
		case EState::eAvoidance:
			Update_Avoidance();
			break;
		// 攻撃
		case EState::eAttack:
			Update_Attack();
			break;
		// 攻撃待ち
		case EState::eAttackWait:
			Update_AttackWait();
			break;
		// 攻撃終了
		case EState::eAttackEnd:
			Update_AttackEnd();
			break;
	}

	// 特定の条件をクリアしている場合抜納を切り替える
	Update_SwitchDrawn();

	// 重力を設定
	mMoveSpeed -= CVector(0.0f, GRAVITY, 0.0f);

	// 移動
	Position(Position() + mMoveSpeed);

	// プレイヤーを移動方向へ向ける
	CVector current = VectorZ();
	CVector target = mMoveSpeed;
	target.Y(0.0f);
	target.Normalize();
	CVector forward = CVector::Slerp(current, target, 0.125f);
	Rotation(CQuaternion::LookRotation(forward));

	// キャラクターの更新
	CXCharacter::Update();

	mIsGrounded = false;

#ifdef _DEBUG
	CVector pos = Position();
	CDebugPrint::Print("プレイヤー情報:\n");
	CDebugPrint::Print("座標(X:%f, Y:%f, Z:%f)\n", pos.X(), pos.Y(), pos.Z());

	CVector angles = EulerAngles();
	CDebugPrint::Print("回転値(X:%f, Y:%f, Z:%f)\n", angles.X(), angles.Y(), angles.Z());
	CDebugPrint::Print("フレーム : %.1f\n",GetAnimationFrame());
	if (mIsDrawn) CDebugPrint::Print("抜刀\n");
	else CDebugPrint::Print("納刀\n");
	
	CDebugPrint::Print("プレイヤー状態 : ");
	if (mState == EState::eIdle)			CDebugPrint::Print("待機状態\n");
	else if (mState == EState::eMove)		CDebugPrint::Print("移動状態\n");
	else if (mState == EState::eFastMove)	CDebugPrint::Print("ダッシュ状態\n");
	else if (mState == EState::eAvoidance)	CDebugPrint::Print("回避状態\n");
	else if (mState == EState::eAttack)		CDebugPrint::Print("攻撃状態\n");
	else if (mState == EState::eAttackWait)	CDebugPrint::Print("攻撃待ち状態\n");
	else if (mState == EState::eAttackEnd)	CDebugPrint::Print("攻撃終了状態\n");

#endif
}

// 衝突処理
void CPlayer::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	if (self == mpColliderLine)
	{
		if (other->Layer() == ELayer::eField)
		{
			mMoveSpeed.Y(0.0f);
			// めり込まないように調整
			Position(Position() + hit.adjust);
			mIsGrounded = true;
			

			// 納刀時の空中待機時
			if (AnimationIndex() == (int)EAnimType::eIdleAir)
			{
				// 着地時に移動キーを押していた場合、着地モーションをローリングに設定
				if (CInput::Key('W') || CInput::Key('A') || CInput::Key('S') || CInput::Key('D'))
				{
					mState = EState::eAvoidance;
					ChangeAnimation(EAnimType::eRollStart);
				}
				else
				{
					// 着地動作(納刀)を再生
					ChangeAnimation(EAnimType::eLanding);
				}
			}
			// 抜刀時の空中待機または、空中攻撃終了時
			else if (AnimationIndex() == (int)EAnimType::eIdleAir_Combat ||
				AnimationIndex() == (int)EAnimType::eAirAttackWait1_1 || AnimationIndex() == (int)EAnimType::eAirAttackEnd1_1 ||
				AnimationIndex() == (int)EAnimType::eAirAttackWait1_2 || AnimationIndex() == (int)EAnimType::eAirAttackEnd1_2 ||
				AnimationIndex() == (int)EAnimType::eAirAttackWait1_3 || AnimationIndex() == (int)EAnimType::eAirAttackEnd1_3 ||
				AnimationIndex() == (int)EAnimType::eAirAttackEnd1_4)
			{
				// 移動キーを押していた場合、着地モーションをローリングに設定
				if (CInput::Key('W') || CInput::Key('A') || CInput::Key('S') || CInput::Key('D'))
				{
					mState = EState::eAvoidance;
					ChangeAnimation(EAnimType::eRollStart);
					mIsAirAttack = false;
				}
				else
				{
					// 着地動作(抜刀)を再生
					ChangeAnimation(EAnimType::eLandin_Combat);
					mIsAirAttack = false;
				}
			}

			if (other->Tag() == ETag::eRideableObject)
			{
				mpRideObject = other->Owner();
			}
		}
	}
}

// 描画
void CPlayer::Render()
{
	CXCharacter::Render();
}

// 納刀中か抜刀中か判定
bool CPlayer::IsDrawn()
{
	return mIsDrawn;
}