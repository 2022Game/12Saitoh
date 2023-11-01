//プレイヤークラスのインクルード
#include "CPlayer.h"
#include "CInput.h"
#include "CCamera.h"
#include "CDebugPrint.h"

// プレイヤーのインスタンス
CPlayer* CPlayer::spInstance = nullptr;

// プレイヤーのモデルデータのパス
#define MODEL_PATH "Character\\Player\\player.x"

// プレイヤーのアニメーションデータのテーブル
const CPlayer::AnimData CPlayer::ANIM_DATA[] =
{
	{ "",													true,	0.0f	},	// Tポーズ
	{ "Character\\Player\\anim\\idle.x",					true,	601.0f	},	// 待機(納刀)
	{ "Character\\Player\\anim\\idle_combat.x",				true,	601.0f	},	// 待機(抜刀)
	{ "Character\\Player\\anim\\idle_drawn_combat.x",		false,	101.0f	},	// 待機中抜刀動作
	{ "Character\\Player\\anim\\idle_sheathed_combat.x",	false,	81.0f	},	// 待機中納刀動作
	{ "Character\\Player\\anim\\run_start.x",				false,	36.0f	},	// 走り開始(納刀)
	{ "Character\\Player\\anim\\run_loop.x",				true,	40.0f	},	// 走り(納刀)
	{ "Character\\Player\\anim\\run_end.x",					false,	50.0f	},	// 走り終了(納刀)
	{ "Character\\Player\\anim\\run_combat_start.x",		false,	36.0f	},	// 走り開始(抜刀)
	{ "Character\\Player\\anim\\run_combat.x",				true,	40.0f	},	// 走り(抜刀)
	{ "Character\\Player\\anim\\run_combat_end.x",			false,	50.0f	},	// 走り終了(抜刀)
	{ "Character\\Player\\anim\\run_drawn_combat.x",		false,	40.0f	},	// 走り中抜刀動作
	{ "Character\\Player\\anim\\run_sheathed_combat.x",		false,	40.0f	},	// 走り中納刀動作
	{ "Character\\Player\\anim\\fastrun_start.x",			false,	11.0f	},	// ダッシュ開始
	{ "Character\\Player\\anim\\fastrun_loop.x",			true,	28.0f	},	// ダッシュ	
	{ "Character\\Player\\anim\\fastrun_end.x",				false,	52.0f	},	// ダッシュ終了
	{ "Character\\Player\\anim\\roll_start.x",				false,	20.0f	},	// 回避動作開始
	{ "Character\\Player\\anim\\roll_end_idle.x",			false,	20.0f	},	// 回避動作からアイドルへ移行	
	{ "Character\\Player\\anim\\roll_end_run.x",			false,	19.0f	},	// 回避動作から走りへ移行
	{ "Character\\Player\\anim\\run_attack.x",				false,	125.0f	},	// 走り後攻撃
	{ "Character\\Player\\anim\\attack_normal_1-1.x",		false,	24.0f	},	// 通常攻撃1-1
	{ "Character\\Player\\anim\\attackwait_normal_1-1.x",	false,	39.0f	},	// 通常攻撃1-1攻撃待ち
	{ "Character\\Player\\anim\\attackend_normal_1-1.x",	false,	30.0f	},	// 通常攻撃1-1終了
	{ "Character\\Player\\anim\\attack_normal_1-2.x",		false,	22.0f	},	// 通常攻撃1-2
	{ "Character\\Player\\anim\\attackwait_normal_1-2.x",	false,	31.0f	},	// 通常攻撃1-2攻撃待ち
	{ "Character\\Player\\anim\\attackend_normal_1-2.x",	false,	30.0f	},	// 通常攻撃1-2終了
	{ "Character\\Player\\anim\\attack_normal_1-3.x",		false,	56.0f	},	// 通常攻撃1-3
	{ "Character\\Player\\anim\\attackwait_normal_1-3.x",	false,	28.0f	},	// 通常攻撃1-3攻撃待ち
	{ "Character\\Player\\anim\\attackend_normal_1-3.x",	false,	30.0f	},	// 通常攻撃1-3終了
	//{ "Character\\Player\\anim\\jump_start.x",	false,	25.0f	},	// ジャンプ開始
	//{ "Character\\Player\\anim\\jump.x",		true,	1.0f	},	// ジャンプ中
	//{ "Character\\Player\\anim\\jump_end.x",	false,	26.0f	},	// ジャンプ終了
};

#define PLAYER_HEIGHT 1.8f
#define MOVE_SPEED 1.0f			// 走る速度
#define FASTMOVE_SPEED 1.5f		// ダッシュ速度
#define ROLL_SPEED 1.3f			// 回避速度
#define ATTACK_SPEED 0.5f		// 攻撃時の移動速度
#define JUMP_SPEED 1.5f
#define GRAVITY 0.0625f
#define JUMP_END_Y 1.0f
// プレイヤー抜刀状態へ切り替えるフレーム(待機状態中)
#define SWITCH_DRAWN_IDLE_FRAME 35
// プレイヤー納刀状態へ切り替えるフレーム(待機状態中)
#define SWITCH_SHEATHED_IDLE_FRAME 31
// プレイヤー抜刀状態へ切り替えるフレーム(走り中)
#define SWITCH_DRAWN_RUN_FRAME 13
// プレイヤー納刀状態へ切り替えるフレーム(走り中)
#define SWITCH_SHEATHED_RUN_FRAME 20

// プレイヤー通常攻撃1-1の移動するフレーム区間(始め)
#define NORMALATTACK1_1_START_FRAME	1
// プレイヤー通常攻撃1-1の移動するフレーム区間(終わり)
#define NORMALATTACK1_1_END_FRAME 15
// プレイヤー通常攻撃1-1の移動速度
#define NORMALATTACK1_1_MOVESPEED 0.5
// プレイヤー通常攻撃1-2の移動するフレーム区間(始め)
#define NORMALATTACK1_2_START_FRAME 0
// プレイヤー通常攻撃1-2の移動するフレーム区間(終わり)
#define NORMALATTACK1_2_END_FRAME 12
// プレイヤー通常攻撃1-2の移動速度
#define NORMALATTACK1_2_MOVESPEED 0.5
// プレイヤー通常攻撃1-3の移動するフレーム区間(始め)
#define NORMALATTACK1_3_START_FRAME 7
// プレイヤー通常攻撃1-3の移動するフレーム区間(終わり)
#define NORMALATTACK1_3_END_FRAME 30
// プレイヤー通常攻撃1-3の移動速度
#define NORMALATTACK1_3_MOVESPEED 0.5

// コンストラクタ
CPlayer::CPlayer()
	: CXCharacter(ETag::ePlayer, ETaskPriority::ePlayer)
	, mState(EState::eIdle)
	, mInput_save(CVector::zero)
	, mIsGrounded(false)
	, mIsDrawn(false)
	, mpRideObject(nullptr)
{
	//インスタンスの設定
	spInstance = this;

	// モデルデータ読み込み
	CModelX* model = new CModelX();
	model->Load(MODEL_PATH);

	// テーブル内のアニメーションデータを読み込み
	int size = ARRAY_SIZE(ANIM_DATA);
	for (int i = 0; i < size; i++)
	{
		const AnimData& data = ANIM_DATA[i];
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
	if (mpColliderLine != nullptr)
	{
		delete mpColliderLine;
		mpColliderLine = nullptr;
	}
	if (mpModel != nullptr)
	{
		delete mpModel;
		mpModel = nullptr;
	}
	if (mpSword != nullptr)
	{
		delete mpSword;
		mpSword = nullptr;
	}
}

CPlayer* CPlayer::Instance()
{
	return spInstance;
}

// アニメーション切り替え
void CPlayer::ChangeAnimation(EAnimType type)
{
	if (!(EAnimType::None < type && type < EAnimType::Num)) return;
	AnimData data = ANIM_DATA[(int)type];
	CXCharacter::ChangeAnimation((int)type, data.loop, data.frameLength);
}

// 抜納状態を切り替える
void CPlayer::SwitchDrawn()
{
	// 抜刀状態の時、納刀状態へ切り替える
	if (mIsDrawn)
	{
		mIsDrawn = false;
	}
	else // 納刀状態の時、抜刀状態へ切り替える
	{
		mIsDrawn = true;
	}
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
			// 左クリックで攻撃状態へ移行
			if (CInput::PushKey(VK_LBUTTON))
			{
				mState = EState::eAttack;
				ChangeAnimation(EAnimType::eNormalAttack1_1);
			}
			// Eキーで納刀
			if (CInput::PushKey('E'))
			{
				ChangeAnimation(EAnimType::eIdle_Sheathed_Combat);
			}
		}
		else
		{
			ChangeAnimation(EAnimType::eIdle);
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
				// 左クリックで抜刀
				if (CInput::PushKey(VK_LBUTTON))
				{
					// 抜刀アニメーションを再生
					ChangeAnimation(EAnimType::eIdle_Drawn_Combat);
				}
			}
		}
		else
		{
			// ひとまずは、待機モーションを追加
			// 飛び降り中のモーションを追加予定
			ChangeAnimation(EAnimType::eIdle);
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
			if (CInput::Key('W'))		input.Z(1.0f);
			else if (CInput::Key('S'))	input.Z(-1.0f);
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
					ChangeAnimation(EAnimType::eRollStart);
				}
				// 左クリックで攻撃状態へ移行
				if (CInput::PushKey(VK_LBUTTON))
				{
					mState = EState::eAttack;
					ChangeAnimation(EAnimType::eNormalAttack1_1);
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
			/* ひとまずは、待機モーションを追加飛び降り中のモーションを追加予定*/
			ChangeAnimation(EAnimType::eIdle);
		}
		break;
	case false:	// 納刀
		// 地面に接地しているか判定
		if (mIsGrounded)
		{
			// 移動処理
			// キーの入力ベクトルを取得
			CVector input;
			if (CInput::Key('W'))		input.Z(1.0f);
			else if (CInput::Key('S'))	input.Z(-1.0f);
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
				// 左クリックで抜刀
				if (CInput::PushKey(VK_LBUTTON))
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
			/* ひとまずは、待機モーションを追加飛び降り中のモーションを追加予定*/
			ChangeAnimation(EAnimType::eIdle);
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
		if (CInput::Key('W'))		input.Z(1.0f);
		else if (CInput::Key('S'))	input.Z(-1.0f);
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
		ChangeAnimation(EAnimType::eIdle);
	}
}

// 回避動作
void CPlayer::Update_Avoidance()
{
	mMoveSpeed.X(0.0f);
	mMoveSpeed.Z(0.0f);

	// キーの入力ベクトルを取得
	CVector input;

	if (CInput::Key('W'))		input.Z(1.0f);
	else if (CInput::Key('S'))	input.Z(-1.0f);
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
			ChangeAnimation(EAnimType::eRollEnd_run);
			// ダッシュキーが押されている場合、ダッシュに切り替える(納刀中のみ)
			if (CInput::Key(VK_SHIFT))
			{
				if (mIsDrawn == false)
				{
					mState = EState::eFastMove;
				}
			}
		}
		// キー入力がない場合はアイドル状態へ移行
		else
		{
			mState = EState::eIdle;
			ChangeAnimation(EAnimType::eRollEnd_idle);
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
	CVector input;
	if (CInput::Key('W'))		input.Z(1.0f);
	else if (CInput::Key('S'))	input.Z(-1.0f);
	if (CInput::Key('A'))		input.X(-1.0f);
	else if (CInput::Key('D'))	input.X(1.0f);

	// 仮保存の入力ベクトルが初期値の場合
	if (mInput_save == CVector::zero)
	{
		// 入力ベクトルデータを一時的に保存
		mInput_save = input;
	}
	// 攻撃アニメーションが終了したら
	// 一時的に保存した入力ベクトルを初期化する
	if (IsAnimationFinished())
	{
		mInput_save = CVector::zero;
	}
	// カメラの向きに合わせた移動ベクトルに変換
	CVector move = CCamera::MainCamera()->Rotation() * mInput_save;
	move.Y(0.0f);
	move.Normalize();

	switch (AnimationIndex())
	{
	case (int)EAnimType::eNormalAttack1_1:	// 通常攻撃1-1処理
		if (IsAnimationFinished())
		{
			mState = EState::eAttackWait;
			ChangeAnimation(EAnimType::eNormalWait1_1);
		}
		// 攻撃に合わせてプレイヤーを移動させる
		if (NORMALATTACK1_1_START_FRAME <= GetAnimationFrame() &&
			GetAnimationFrame() <= NORMALATTACK1_1_END_FRAME)
		{
			mMoveSpeed += move * NORMALATTACK1_1_MOVESPEED;
		}
		break;
	case (int)EAnimType::eNormalAttack1_2:	// 通常攻撃1-2処理
		if (IsAnimationFinished())
		{
			mState = EState::eAttackWait;
			ChangeAnimation(EAnimType::eNormalWait1_2);
		}
		// 攻撃に合わせてプレイヤーを移動させる
		if (NORMALATTACK1_2_START_FRAME <= GetAnimationFrame() &&
			GetAnimationFrame() <= NORMALATTACK1_2_END_FRAME)
		{
			mMoveSpeed += move * NORMALATTACK1_2_MOVESPEED;
		}
		break;
	case (int)EAnimType::eNormalAttack1_3:	// 通常攻撃1-3処理
		if (IsAnimationFinished())
		{
			mState = EState::eAttackWait;
			ChangeAnimation(EAnimType::eNormalWait1_3);
		}
		// 攻撃に合わせてプレイヤーを移動させる
		if (NORMALATTACK1_3_START_FRAME <= GetAnimationFrame() &&
			GetAnimationFrame() <= NORMALATTACK1_3_END_FRAME)
		{
			mMoveSpeed += move * NORMALATTACK1_3_MOVESPEED;
		}
	}

}

// 攻撃終了待ち
void CPlayer::Update_AttackWait()
{
	switch (AnimationIndex())
	{
	case (int)EAnimType::eNormalWait1_1:	// 通常攻撃1-1攻撃待ち
		if (CInput::PushKey(VK_LBUTTON))
		{
			// 通常攻撃1-2へ切り替え
			mState = EState::eAttack;
			ChangeAnimation(EAnimType::eNormalAttack1_2);
		}
		if (IsAnimationFinished())
		{
			mState = EState::eAttackEnd;
			ChangeAnimation(EAnimType::eNormalEnd1_1);
		}
		break;
	case (int)EAnimType::eNormalWait1_2:	// 通常攻撃1-2攻撃待ち
		if (CInput::PushKey(VK_LBUTTON))
		{
			// 通常攻撃1-3へ切り替え
			mState = EState::eAttack;
			ChangeAnimation(EAnimType::eNormalAttack1_3);
		}
		if (IsAnimationFinished())
		{
			mState = EState::eAttackEnd;
			ChangeAnimation(EAnimType::eNormalEnd1_2);
		}
		break;
	case (int)EAnimType::eNormalWait1_3:	// 通常攻撃1-3攻撃待ち
		if (CInput::PushKey(VK_LBUTTON))
		{
			// 通常攻撃1-1へ切り替え
			mState = EState::eAttack;
			ChangeAnimation(EAnimType::eNormalAttack1_1);
		}
		if (IsAnimationFinished())
		{
			mState = EState::eAttackEnd;
			ChangeAnimation(EAnimType::eNormalEnd1_3);
		}
	}
	// 攻撃待ちモーションの間に何も入力がなければ、
	// アイドル状態に移行する
	if (IsAnimationFinished())
	{
		// 待機状態へ移行
		mState = EState::eIdle;
	}

	// 攻撃待ちモーション中に移動＋回避キーの入力があれば
	// 回避状態へ移行する
	if (CInput::Key('W') || CInput::Key('A') ||
		CInput::Key('S') || CInput::Key('D'))
	{
		if (CInput::PushKey(VK_SPACE))
		{
			mState = EState::eAvoidance;
			ChangeAnimation(EAnimType::eRollStart);
		}
	}
}

// 攻撃終了
void CPlayer::Update_AttackEnd()
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
			ChangeAnimation(EAnimType::eRollStart);
		}
	}
	// 攻撃終了モーションが終了したら待機状態へ移行する
	if (IsAnimationFinished())
	{
		mState = EState::eIdle;
	}
}

// ジャンプ開始
void CPlayer::Update_JumpStart()
{
	ChangeAnimation(EAnimType::eJumpStart);
	mState = EState::eJump;

	mMoveSpeed += CVector(0.0f, JUMP_SPEED, 0.0f);
	mIsGrounded = false;
}

// ジャンプ中
void CPlayer::Update_Jump()
{
	if (mMoveSpeed.Y() <= 0.0f)
	{
		ChangeAnimation(EAnimType::eJumpEnd);
		mState = EState::eJumpEnd;
	}
}

// ジャンプ終了
void CPlayer::Update_JumpEnd()
{
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
		// ジャンプ開始
		case EState::eJumpStart:
			Update_JumpStart();
			break;
		// ジャンプ中
		case EState::eJump:
			Update_Jump();
			break;
		// ジャンプ終了
		case EState::eJumpEnd:
			Update_JumpEnd();
			break;
	}

	// 特定の条件をクリアしている場合抜納を切り替える
	Update_SwitchDrawn();

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
	if (mIsDrawn){
		CDebugPrint::Print("状態 : 抜刀");
	}
	else{
		CDebugPrint::Print("状態 : 納刀");
	}
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
			Position(Position() + hit.adjust);
			mIsGrounded = true;

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