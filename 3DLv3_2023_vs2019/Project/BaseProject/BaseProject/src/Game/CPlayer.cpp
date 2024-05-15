//プレイヤークラスのインクルード
#include "CPlayer.h"
#include "CDebugPrint.h"
#include "CHPGauge.h"
#include "CSPGauge.h"
#include "CEnemy.h"
#include "CColliderSphere.h"
#include "CColliderCapsule.h"
#include "CSword.h"

#include "CFlamethrower.h"
// プレイヤーのインスタンス
CPlayer* CPlayer::spInstance = nullptr;

// コンストラクタ
CPlayer::CPlayer()
	: CXCharacter(ETag::ePlayer, ETaskPriority::ePlayer,
		0, ETaskPauseType::ePlayer)
	, mState(EState::eIdle)
	, mInput_save(CVector::zero)
	, mMoveSpeed(CVector::zero)
	, mCamForward(CVector::zero)
	, mCamSide(CVector::zero)
	, mIsGrounded(false)
	, mIsDrawn(false)
	, mIsAirAttack(false)
	, mIsCounter(false)
	, mIsDash(false)
	, mIsAvoid(false)
	, mSPZeroFlag(false)
	, mIsUpdateInput(false)
	, mStateStep(0)
	, mAttackStep(0)
	, mSPAttackStep(0)
	, mHPRecoveryTime(0.0f)
	, mpRideObject(nullptr)
{
	//インスタンスの設定
	spInstance = this;

	// モデルデータ取得
	CModelX* model = CResourceManager::Get<CModelX>("Player");

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

	//線分コライダの設定
	mpColliderLine = new CColliderLine
	(
		this, ELayer::eField,
		CVector(0.0f, 0.0f, 0.0f),
		CVector(0.0f, PLAYER_HEIGHT, 0.0f)
	);
	mpColliderLine->SetCollisionLayers({ ELayer::eField });
	mpColliderLine2 = new CColliderLine
	(
		this, ELayer::eField,
		CVector(-0.5f,1.0f,0.0f),
		CVector(0.5f, 1.0f, 0.0f)
	);
	mpColliderLine2->SetCollisionLayers({ ELayer::eField });
	mpColliderLine3 = new CColliderLine
	(
		this, ELayer::eField,
		CVector(0.0f, 1.0f, -0.5f),
		CVector(0.0f, 1.0f, 0.5f)
	);
	mpColliderLine3->SetCollisionLayers({ ELayer::eField });

	// コライダの生成
	// 押し戻し用コライダ
	mpBodyCol = new CColliderSphere(this, ELayer::ePlayer, 0.5f);
	mpBodyCol->SetCollisionLayers({ ELayer::eEnemy });
	mpBodyCol->SetCollisionTags({ ETag::eEnemy });
	mpBodyCol->Position(0.0f, 1.0f, 0.0f);

	// ダメージ用コライダ
	mpDamageCol = new CColliderSphere(this, ELayer::eDamageCol, 0.7f);
	mpDamageCol->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageCol->SetCollisionTags({ ETag::eEnemy, ETag::eFlame });
	mpDamageCol->Position(0.0f, 1.0f, 0.0f);

	// カットインカメラの生成
	mpCutIn_PowerAttack = new CCutIn_PowerAttack();

	// プレイヤーのステータスを取得
	mStatus = PLAYER_STATUS[PLAYER_STATAS];
	mMaxStatus = mStatus;
	mStatus.touki = 0;

	// HPゲージを作成
	mpHPGauge = new CHPGauge();
	mpHPGauge->SetPos(10.0f, 10.0f);
	mpHPGauge->SetMaxValue(mStatus.hp);

	// SPゲージを作成
	mpSPGauge = new CSPGauge();
	mpSPGauge->SetPos(10.0f, 40.0f);
	mpSPGauge->SetMaxValue(mStatus.sp);

	//剣を作成
	mpSword = new CSword();
	mpSword->Rotate(CVector(0.0f, 0.0f, 0.0f));

	mpFlamethrower = new CFlamethrower
	(
		this, nullptr,
		CVector(0.0f, 14.0f, -1.0f)
	);
}

CPlayer::~CPlayer()
{
	SAFE_DELETE(mpColliderLine);
	SAFE_DELETE(mpColliderLine2);
	SAFE_DELETE(mpColliderLine3);
	SAFE_DELETE(mpBodyCol);

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
	CXCharacter::ChangeAnimation((int)type, data.loop, data.frameLength, data.motionValue);
}

// ステータスの切り替え
void CPlayer::ChangeState(EState state)
{
	mState = state;
	mStateStep = 0;
}

// 抜納状態を切り替える
void CPlayer::SwitchDrawn()
{
	// 抜刀状態の時、納刀状態へ切り替える
	if (mIsDrawn) mIsDrawn = false;
	// 納刀状態の時、抜刀状態へ切り替える
	else mIsDrawn = true;
}

// 抜納の切り替え処理
void CPlayer::Update_SwitchDrawn()
{
	switch (mStateStep)
	{
	case 0:
		EAnimType animType;
		float animFrame;
		// 特定のアニメーションで抜納状態を切り替える
		// アニメーション中のフレームによって切り替えるタイミングを決める
		if (mIsDrawn)// 抜刀中
		{
			if (mState == EState::eIdle)// アイドル中
			{
				animType = EAnimType::eIdle_Sheathed_Combat;
				animFrame = SWITCH_SHEATHED_IDLE_FRAME;
			}
			else// 走り中
			{
				animType = EAnimType::eRun_Sheathed_Combat;
				animFrame = SWITCH_SHEATHED_RUN_FRAME;
			}

			// 納刀する
			if (AnimationIndex() == (int)animType &&
				GetAnimationFrame() >= animFrame)
			{
				SwitchDrawn();
				mStateStep++;
			}
		}
		else// 納刀中
		{
			if (mState == EState::eIdle)// アイドル中
			{
				animType = EAnimType::eIdle_Drawn_Combat;
				animFrame = SWITCH_DRAWN_IDLE_FRAME;
			}
			else// 走り中
			{
				animType = EAnimType::eRun_Drawn_Combat;
				animFrame = SWITCH_DRAWN_RUN_FRAME;
			}

			// 抜刀する
			if (AnimationIndex() == (int)animType &&
				GetAnimationFrame() >= animFrame)
			{
				SwitchDrawn();
				mStateStep++;
			}
		}
		break;
	case 1:
		break;
	}
}

// 更新
void CPlayer::Update()
{
	// カメラの正面・サイドベクトルを設定
	CCamera* mainCamera = CCamera::MainCamera();
	mCamForward = mainCamera->VectorZ();
	mCamSide = CVector::Cross(CVector::up, mCamForward);

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

	// スタミナ回復処理
	// ダッシュまたは回避中以外の時は実行
	if (!mIsDash && !mIsAvoid)
	{
		if (mStatus.sp <= PLAYER_MAX_SP)
		{
			mStatus.sp += 0.3;
		}
		if (mStatus.sp >= PLAYER_MAX_SP)
		{
			// SPが全回復
			mSPZeroFlag = false;
		}
	}

	// 各ステータスの上限値と下限値を設定
	// 上限
	if (mStatus.hp >= mMaxStatus.hp) mStatus.hp = mMaxStatus.hp; // HP
	if (mStatus.sp >= mMaxStatus.sp) mStatus.sp = mMaxStatus.sp; // SP
	if (mStatus.touki >= mMaxStatus.touki) mStatus.touki = mMaxStatus.touki;// 闘気
	// 下限
	if (mStatus.hp <= PLAYER_MIN_HP) mStatus.hp = PLAYER_MIN_HP; // HP
	if (mStatus.sp <= PLAYER_MIN_SP) mStatus.sp = PLAYER_MIN_SP; // SP
	if (mStatus.touki <= PLAYER_MIN_TOUKI) mStatus.touki = PLAYER_MIN_TOUKI;// 闘気

	if (mpCutIn_PowerAttack->IsPlaying())
	{
		// キャラクターの更新
		CXCharacter::Update();
		return;
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
	CDebugPrint::Print("FPS:%f\n", Time::FPS());
	CVector pos = Position();
	CDebugPrint::Print("プレイヤー情報:\n");
	CDebugPrint::Print("座標(X:%f, Y:%f, Z:%f)\n", pos.X(), pos.Y(), pos.Z());

	//CVector angles = EulerAngles();
	//CDebugPrint::Print("回転値(X:%f, Y:%f, Z:%f)\n", angles.X(), angles.Y(), angles.Z());
	//CDebugPrint::Print("アニメーションフレーム : %.1f\n",GetAnimationFrame());
	if (mIsDrawn) CDebugPrint::Print("抜刀\n");
	else CDebugPrint::Print("納刀\n");
	
	CDebugPrint::Print("プレイヤー状態 : ");
	if (mState == EState::eIdle)			CDebugPrint::Print("待機状態\n");
	else if (mState == EState::eMove)		CDebugPrint::Print("移動状態\n");
	else if (mState == EState::eFastMove)	CDebugPrint::Print("ダッシュ状態\n");
	else if (mState == EState::eAvoidance)	CDebugPrint::Print("回避状態\n");
	else if (mState == EState::eAttack)		CDebugPrint::Print("攻撃状態\n");
	else if (mState == EState::eSpecalMove) CDebugPrint::Print("闘技状態\n");

	CDebugPrint::Print("  HP 　: %d\n", mStatus.hp);
	CDebugPrint::Print("攻撃力 : %d\n", mStatus.atk);
	CDebugPrint::Print("防御力 : %d\n", mStatus.def);
	CDebugPrint::Print("スタミナ : %.0f\n", mStatus.sp);
	CDebugPrint::Print("闘気ゲージ : %d\n", mStatus.touki);

	CDebugPrint::Print("攻撃段階 : %d\n", mAttackStep);
	
	//// カウンター攻撃フラグの変更
	//if (CInput::PushKey('Q')) mIsCounter = true;
	//
	//CDebugPrint::Print("カウンターフラグ : ");
	//if (mIsCounter) CDebugPrint::Print("ture\n");
	//else CDebugPrint::Print("false\n");

	//  1キーを押しながら、「↑」キーでHP増加 「↓」でHP減少
	if (CInput::Key('1'))
	{
		if (CInput::Key(VK_UP)) mStatus.hp++;
		else if (CInput::Key(VK_DOWN)) mStatus.hp--;
	}
	// 2キーを押しながら、「↑」キーでSP増加 「↓」でSP減少
	if (CInput::Key('2'))
	{
		if (CInput::Key(VK_UP)) mStatus.sp++;
		else if (CInput::Key(VK_DOWN)) mStatus.sp--;
	}

	//CDebugPrint::Print("モーション値 : %.2f\n", mMotionValue);

	//CDebugPrint::Print("暫定ダメージ : %d\n", mTemporaryDamage);
	//if (mTemporaryDamage > 0)
	//{
	//	if (mHPRecoveryTime >= 2)
	//	{
	//		mTemporaryDamage--;
	//		mStatus.hp++;
	//		mHPRecoveryTime = 0;
	//	}
	//	else {
	//		mHPRecoveryTime += 0.016666f;
	//	}
	//}

#endif
	// HPゲージに現在のHPを設定
	mpHPGauge->SetValue(mStatus.hp);
	// SPゲージに現在のSPを設定
	mpSPGauge->SetValueF(mStatus.sp);
	mpSPGauge->SetSPZeroFlag(mSPZeroFlag);
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
					ChangeState(EState::eAvoidance);
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
					ChangeState(EState::eAvoidance);
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
	// 壁との衝突処理
	if (self == mpColliderLine2)
	{
		if (other->Layer() == ELayer::eField)
		{
			mMoveSpeed.X(0.0f);
			Position(Position() + hit.adjust);
		}
	}
	if (self == mpColliderLine3)
	{
		if (other->Layer() == ELayer::eField)
		{
			mMoveSpeed.Z(0.0f);
			Position(Position() + hit.adjust);
		}
	}

	// 敵との衝突処理
	if (self == mpBodyCol)
	{
		Position(Position() +
			CVector(hit.adjust.X(), 0.0f, hit.adjust.Z()) * hit.weight);
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

// ダメージ処理
void CPlayer::TakeDamage(int damage)
{
	mStatus.hp -= damage;
	if (mStatus.hp <= 0)
	{
		// 死亡処理
	}
	else
	{
		// のけ反る処理
	}
}