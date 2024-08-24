//プレイヤークラスのインクルード
#include "CPlayer.h"
#include "CDebugPrint.h"
#include "CHPGauge.h"
#include "CSPGauge.h"
#include "CToukiGauge.h"
#include "CEnemy.h"
#include "CColliderCapsule.h"
#include "CSword.h"
#include "CFlamethrower.h"
#include "CDragon.h"
#include "Global.h"
#include "CGameUI.h"
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
	, mIsInvincible(false)
	, mIsDie(false)
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
	mpBodyCol = new CColliderCapsule(this, ELayer::ePlayer,
		CVector(0.0f, 0.2f, 0.0f),CVector(0.0f, 1.3f, 0.0f),3.0f);
	mpBodyCol->SetCollisionLayers({ ELayer::eEnemy });
	mpBodyCol->SetCollisionTags({ ETag::eEnemy });

	// ダメージ用コライダ
	mpDamageCol = new CColliderCapsule(this, ELayer::eDamageCol,
		CVector(0.0f, 0.2f, 0.0f),CVector(0.0f, 1.5f, 0.0f),3.0f);
	mpDamageCol->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageCol->SetCollisionTags({ ETag::eEnemy, ETag::eFlame });

	// カットインカメラの生成
	mpCutIn_PowerAttack = new CCutIn_PowerAttack();
	// ゲームオーバーカットインカメラの生成
	mpCutIn_GameOver = new CCutIn_GameOver();
	// ターゲットカメラの生成
	mpTargetCamera = new CTargetCamera();
	mpTargetCamera->AddCollider(gField->GetFieldCol());


	// プレイヤーのステータスを取得
	mStatus = PLAYER_STATUS[PLAYER_STATAS];
	mMaxStatus = mStatus;

	// HPゲージを作成
	CGameUI::SetMaxHP(mStatus.hp);

	// SPゲージを作成
	CGameUI::SetMaxSP(mStatus.sp);

	// 闘気ゲージを作成
	CGameUI::SetMaxTouki(mStatus.touki);

	// 闘気は0にする
	mStatus.touki = 0;

	//剣を作成
	mpSword = new CSword();
	mpSword->Rotate(CVector(0.0f, 0.0f, 0.0f));

	// SE読み込み
	mpRunSE = CResourceManager::Get<CSound>("RunSE");
	mpFastRunSE = CResourceManager::Get<CSound>("FastRunSE");
	mpNormalAttackSE1 = CResourceManager::Get<CSound>("NormalAttackSE1");
	mpNormalAttackSE2 = CResourceManager::Get<CSound>("NormalAttackSE2");
	mpSpMoveSE = CResourceManager::Get<CSound>("SpMoveSE");
	mpMissSE = CResourceManager::Get<CSound>("SpMissSE");
	mpNormalAttackSE1->SetSimultaneousPlayCount(10);
	mpNormalAttackSE2->SetSimultaneousPlayCount(10);
	mpSpMoveSE->SetSimultaneousPlayCount(10);
	mpMissSE->SetSimultaneousPlayCount(1);
}

CPlayer::~CPlayer()
{
	SAFE_DELETE(mpColliderLine);
	SAFE_DELETE(mpColliderLine2);
	SAFE_DELETE(mpColliderLine3);
	SAFE_DELETE(mpBodyCol);
	SAFE_DELETE(mpDamageCol);

	mpCutIn_PowerAttack->Kill();
	mpSword->Kill();
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

	// SEの音量を調整
	// SEを停止すると、次の再生が出来ないため音量を変えて調整
	if (mpRunSE->IsPlaying()) mpRunSE->SetVolume(0.0f);
	if (mpFastRunSE->IsPlaying()) mpFastRunSE->SetVolume(0.0f);
}

// 抜納状態を切り替える
void CPlayer::SwitchDrawn()
{
	// 抜刀状態の時、納刀状態へ切り替える
	if (mIsDrawn)
	{
		mIsDrawn = false;
		CSound* sheathedSE = CResourceManager::Get<CSound>("SheathedSE");
		sheathedSE->Play(0.1f);
	}
	// 納刀状態の時、抜刀状態へ切り替える
	else
	{
		mIsDrawn = true;
		CSound* drawnSE = CResourceManager::Get<CSound>("DrawnSE");
		drawnSE->Play(0.07f);
	}
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
	CCamera* CurrentCamera = CCamera::CurrentCamera();
	mCamForward = CurrentCamera->VectorZ();
	mCamSide = CVector::Cross(CVector::up, mCamForward);

	SetParent(mpRideObject);
	mpRideObject = nullptr;

	// ターゲットカメラの切り替え処理
	if (!mpTargetCamera->IsPlaying() &&
		!mpCutIn_PowerAttack->IsPlaying())
	{
		if (CInput::PushKey('Q')) 
		{
			mpTargetCamera->Start();
			CDragon* dragon = CDragon::Instance();
			mpTargetCamera->Update_Set(this, dragon);
		}
	}
	// ターゲットカメラが再生中の場合、必要な情報を入れる
	else
	{
		if (CInput::PushKey('Q'))
		{
			mpTargetCamera->End();
		}
		CDragon* dragon = CDragon::Instance();
		mpTargetCamera->Update_Set(this, dragon);
	}

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
		// 死亡
		case EState::eDie:
			Update_Die();
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
	//CDebugPrint::Print("FPS:%f\n", Time::FPS());
	//CVector pos = Position();
	//CDebugPrint::Print("プレイヤー情報:\n");
	//CDebugPrint::Print("座標(X:%f, Y:%f, Z:%f)\n", pos.X(), pos.Y(), pos.Z());

	//CVector angles = EulerAngles();
	//CDebugPrint::Print("回転値(X:%f, Y:%f, Z:%f)\n", angles.X(), angles.Y(), angles.Z());
	//CDebugPrint::Print("アニメーションフレーム : %.1f\n",GetAnimationFrame());
	//if (mIsDrawn) CDebugPrint::Print("抜刀\n");
	//else CDebugPrint::Print("納刀\n");
	//
	//CDebugPrint::Print("プレイヤー状態 : ");
	//if (mState == EState::eIdle)			CDebugPrint::Print("待機状態\n");
	//else if (mState == EState::eMove)		CDebugPrint::Print("移動状態\n");
	//else if (mState == EState::eFastMove)	CDebugPrint::Print("ダッシュ状態\n");
	//else if (mState == EState::eAvoidance)	CDebugPrint::Print("回避状態\n");
	//else if (mState == EState::eAttack)		CDebugPrint::Print("攻撃状態\n");
	//else if (mState == EState::eSpecalMove) CDebugPrint::Print("闘技状態\n");

	//CDebugPrint::Print("  HP 　: %d\n", mStatus.hp);
	//CDebugPrint::Print("攻撃力 : %d\n", mStatus.atk);
	//CDebugPrint::Print("防御力 : %d\n", mStatus.def);
	//CDebugPrint::Print("スタミナ : %.0f\n", mStatus.sp);
	//CDebugPrint::Print("闘気ゲージ : %d\n", mStatus.touki);

	CDebugPrint::Print("暫定ダメージ : %d\n", mTemporaryDamage);

	if (CInput::PushKey('Q')) mIsCounter = true;
	if(CInput::Key(VK_UP)) mStatus.touki++;
	if (CInput::Key(VK_DOWN)) mStatus.hp--;
	if (CInput::PushKey('L'))
	{
		mpCutIn_GameOver->Setup(this);
		mpCutIn_GameOver->Start();
	}
#endif
	if (mTemporaryDamage > 0)
	{
		// 2秒経過するごとにHPを回復
		if (mHPRecoveryTime >= 2)
		{
			mTemporaryDamage--;
			mStatus.hp++;
			mHPRecoveryTime = 0;
		}
		else {
			mHPRecoveryTime += 0.016666f;
		}
	}
	// HPゲージに現在のHPを設定
	CGameUI::SetHP(mStatus.hp);
	// SPゲージに現在のSPを設定
	CGameUI::SetSP(mStatus.sp);
	CGameUI::SetSPZeroFlag(mSPZeroFlag);
	// 闘気ゲージに現在の闘気を設定
	CGameUI::SetTouki(mStatus.touki);
	// 闘気の値を見てUIの表示を変更
	if (mStatus.touki < 100)
	{
		// 両方グレーで表示
		CGameUI::SetColorCounter(CColor::gray);
		CGameUI::SetColorSpMove(CColor::gray);
	}
	else if (mStatus.touki < 200)
	{
		// 弱闘技のみ色を付ける
		CGameUI::SetColorCounter(CColor::white);
		CGameUI::SetColorSpMove(CColor::gray);
	}
	else
	{
		// 両方に色を付ける
		CGameUI::SetColorCounter(CColor::white);
		CGameUI::SetColorSpMove(CColor::white);
	}
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
					mAttackStep = 0;
				}
				else
				{
					// 着地動作(抜刀)を再生
					ChangeState(EState::eIdle);
					ChangeAnimation(EAnimType::eLandin_Combat);
					mIsAirAttack = false;
					mAttackStep = 0;
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
	if (mIsDie) return;
	// カウンター状態中であれば
	if (IsCounter())
	{
		// カウンターフラグを立てる
		mIsCounter = true;
		// ダメージと暫定ダメージは0にする
		damage = 0;
		mTemporaryDamage = 0;
		// カウンター攻撃中は無敵状態にする
		InvincibleON();
	}

	mStatus.hp -= damage;
	if (mStatus.hp <= 0)
	{
		mTemporaryDamage = 0;
		// 死亡処理
		if (IsDrawn())
		{
			// 抜刀中
			ChangeAnimation(EAnimType::eDie_Start_Combat);
		}
		else
		{
			// 納刀中
			ChangeAnimation(EAnimType::eDie_Start_Combat);
		}
		mState = EState::eDie;
		// ダメージ用のコライダーをオフにしておく
		mpDamageCol->SetEnable(false);
		// プレイヤーUIもオフにする
		CGameUI::SetShowUI(false);
		// 死亡フラグを立てる
 		mIsDie = true;
		// ゲームオーバーカットインカメラを再生
		mpCutIn_GameOver->Setup(this);
		mpCutIn_GameOver->Start();
		// ゲームUIを非表示
		CGameUI::SetShowUI(false);

	}
}

// 闘気を増加
void CPlayer::UpTouki()
{
	// 特殊攻撃中で無ければ闘気を増加させる
	if (mState != EState::eSpecalMove)
	{
		mStatus.touki += UP_TOUKI;
	}
}

// 無敵状態にする
void CPlayer::InvincibleON()
{
	mIsInvincible = true;
	// ダメージ判定用のコライダーもOFFにしておく
	mpDamageCol->SetEnable(false);
}

// 無敵状態を解除する
void CPlayer::InvincibleOFF()
{
	mIsInvincible = false;
	// ダメージ判定用のコライダーをONにする
	mpDamageCol->SetEnable(true);
}

// 無敵状態かどうか
bool CPlayer::IsInvincible()const
{
	if (mIsInvincible) return true;
	
	return false;
}

// カウンター中かどうか
bool CPlayer::IsCounter()
{
	if (AnimationIndex() == (int)EAnimType::eCounter_Start) return true;
	else if (AnimationIndex() == (int)EAnimType::eCounter_Loop) return true;

	return false;
}

// 死亡したかどうか
bool CPlayer::IsDie() const
{
	return mIsDie;
}

// 死亡処理
void CPlayer::Update_Die()
{
	mMoveSpeed.X(0.0f);
	mMoveSpeed.Y(0.0f);
	mMoveSpeed.Z(0.0f);
	if (IsAnimationFinished())
	{
		if (AnimationIndex() == (int)EAnimType::eDie_Start)
		{
			ChangeAnimation(EAnimType::eDie_Loop);
		}
		else if (AnimationIndex() == (int)EAnimType::eDie_Start_Combat)
		{
			ChangeAnimation(EAnimType::eDie_Loop_Combat);
		}
	}

	// 各SEが再生中の場合、停止する
	if (mpRunSE->IsPlaying()) mpRunSE->Stop();
	if (mpFastRunSE->IsPlaying()) mpFastRunSE->Stop();
	if (mpNormalAttackSE1->IsPlaying()) mpNormalAttackSE1->Stop();
	if (mpNormalAttackSE2->IsPlaying()) mpNormalAttackSE2->Stop();
	if (mpSpMoveSE->IsPlaying()) mpSpMoveSE->Stop();

	mpCutIn_GameOver->Setup(this);
}

CPlayer::EState CPlayer::GetState() const
{
	return mState;
}