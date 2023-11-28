//プレイヤークラスのインクルード
#include "CPlayer.h"
#include "CDebugPrint.h"

// プレイヤーのインスタンス
CPlayer* CPlayer::spInstance = nullptr;

// コンストラクタ
CPlayer::CPlayer()
	: CXCharacter(ETag::ePlayer, ETaskPriority::ePlayer,
		0, ETaskPauseType::ePlayer)
	, mState(EState::eIdle)
	, mInput_save(CVector::zero)
	, mIsGrounded(false)
	, mIsDrawn(false)
	, mIsAirAttack(false)
	, mIsCounter(false)
	, mAttackStep(0)
	, mpRideObject(nullptr)
{
	//インスタンスの設定
	spInstance = this;

	// モデルデータ読み込み
	CModelX* model = new CModelX();
	model->Load(MODEL_PATH);

	// テーブル内のアニメーションデータを読み込み
	int size = PlayerData::GetAnimDataSize();
	for (int i = 0; i < size; i++)
	{
		const PlayerData::AnimData& data = PlayerData::GetAnimData(i);
		if (data.path.empty()) continue;
		model->AddAnimationSet(data.path.c_str());
	}
	// CXCharacterの初期化
	Init(model);

	// 最初は待機アニメーションを再生
	ChangeAnimation(EAnimType::eIdle);

	// ステータスの設定
	mStatas.atk = PLAYER_ATK;
	mStatas.def = PLAYER_DEF;
	mStatas.hp = PLAYER_HP;
	mStatas.sp = PLAYER_SP;
	mStatas.touki = PLAYER_TOUKI;

	//線分コライダの設定
	mpColliderLine = new CColliderLine
	(
		this, ELayer::eField,
		CVector(0.0f, 0.0f, 0.0f),
		CVector(0.0f, PLAYER_HEIGHT, 0.0f)
	);
	mpColliderLine->SetCollisionLayers({ ELayer::eField });

	mpCutIn_PowerAttack = new CCutIn_PowerAttack();
}

CPlayer::~CPlayer()
{
	SAFE_DELETE(mpColliderLine);
	SAFE_DELETE(mpModel);
	SAFE_DELETE(mpSword);

	mpCutIn_PowerAttack->Kill();
}

CPlayer* CPlayer::Instance()
{
	return spInstance;
}

// アニメーション切り替え
void CPlayer::ChangeAnimation(EAnimType type)
{
	if (!(EAnimType::None < type && type < EAnimType::Num)) return;
	PlayerData::AnimData data = PlayerData::GetAnimData((int)type);
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

// 更新
void CPlayer::Update()
{
	if (mpCutIn_PowerAttack->IsPlaying())
	{
		// キャラクターの更新
		CXCharacter::Update();
		return;
	}

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
		// 闘技
		case EState::eSpecalMove:
			Update_SpecialMove();
			break;
	}

	// 特定の条件をクリアしている場合抜納を切り替える
	Update_SwitchDrawn();

	// 重力を設定
	mMoveSpeed -= CVector(0.0f, GRAVITY, 0.0f);

	// 移動
	Position(Position() + mMoveSpeed);

	if (mState != EState::eSpecalMove)
	{
		// プレイヤーを移動方向へ向ける
		CVector current = VectorZ();
		CVector target = mMoveSpeed;
		target.Y(0.0f);
		target.Normalize();
		CVector forward = CVector::Slerp(current, target, 0.125f);
		Rotation(CQuaternion::LookRotation(forward));
	}

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
	else if (mState == EState::eSpecalMove) CDebugPrint::Print("闘技状態\n");

	CDebugPrint::Print("攻撃力 : %d\n", mStatas.atk);
	CDebugPrint::Print("防御力 : %d\n", mStatas.def);
	CDebugPrint::Print("HP : %d\n", mStatas.hp);
	CDebugPrint::Print("スタミナ : %d\n", mStatas.sp);
	CDebugPrint::Print("闘気ゲージ : %d\n", mStatas.touki);

	CDebugPrint::Print("攻撃段階 : %d\n", mAttackStep);

	if (CInput::PushKey('Q'))
	{
		mIsCounter = true;
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