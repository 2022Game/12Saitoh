//プレイヤークラスのインクルード
#include "CPlayer.h"
#include "CDebugPrint.h"
#include "CHPGauge.h"
#include "CSPGauge.h"
#include "CEnemy.h"

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
	, time(0)

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

	// カットインカメラの生成
	mpCutIn_PowerAttack = new CCutIn_PowerAttack();

	// プレイヤーのステータスを取得
	mStatus = PLAYER_STATUS[PLAYER_STATAS];

	// HPゲージを作成
	mpHPGauge = new CHPGauge();
	mpHPGauge->SetPos(10.0f, 10.0f);
	mpHPGauge->SetMaxValue(mStatus.hp);

	// SPゲージを作成
	mpSPGauge = new CSPGauge();
	mpSPGauge->SetPos(10.0f, 40.0f);
	mpSPGauge->SetMaxValue(0);

	// それぞれのステータスの最大値を設定

}

CPlayer::~CPlayer()
{
	SAFE_DELETE(mpColliderLine);

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
	CXCharacter::ChangeAnimation((int)type, data.loop, data.frameLength, data.MotionValue);
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

	// 各ステータスの上限値と下限値を設定
	// 上限
	if (mStatus.hp > 100) mStatus.hp = 100;	// HP
	if (mStatus.sp > 100) mStatus.sp = 100;	// SP
	if (mStatus.touki > 300) mStatus.touki = 300; // 闘気
	// 加減
	if (mStatus.hp < 0) mStatus.hp = 0;	// HP
	if (mStatus.sp < 0) mStatus.sp = 0;	// SP
	if (mStatus.touki < 0) mStatus.touki = 0; // 闘気

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
	CVector pos = Position();
	CDebugPrint::Print("プレイヤー情報:\n");
	CDebugPrint::Print("座標(X:%f, Y:%f, Z:%f)\n", pos.X(), pos.Y(), pos.Z());

	CVector angles = EulerAngles();
	CDebugPrint::Print("回転値(X:%f, Y:%f, Z:%f)\n", angles.X(), angles.Y(), angles.Z());
	CDebugPrint::Print("アニメーションフレーム : %.1f\n",GetAnimationFrame());
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
	CDebugPrint::Print("スタミナ : %d\n", mStatus.sp);
	CDebugPrint::Print("闘気ゲージ : %d\n", mStatus.touki);

	CDebugPrint::Print("攻撃段階 : %d\n", mAttackStep);
	
	// カウンター攻撃フラグの変更
	if (CInput::PushKey('Q')) mIsCounter = true;
	
	CDebugPrint::Print("カウンターフラグ : ");
	if (mIsCounter) CDebugPrint::Print("ture\n");
	else CDebugPrint::Print("false\n");

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

	CDebugPrint::Print("モーション値 : %.2f\n", mMotionValue);
	if (CInput::Key('1'))
	{
		if (CInput::PushKey(VK_RIGHT))
		{
			//CEnemy* enemy = new CEnemy();
			//int damage = TakePlayerToDamage(
			//	enemy->Instance()->Status().atk,
			//	mStatus.def,
			//	1.6);
			int damage = TakePlayerToDamage(100, mStatus.def, 1.5);
			mStatus.hp -= damage; 
			printf("%d\n", damage);
		}
	}
	CDebugPrint::Print("暫定ダメージ : %d\n", mTemporaryDamage);
	if (mTemporaryDamage > 0)
	{
		if (time >= 2)
		{
			mTemporaryDamage--;
			mStatus.hp++;
			time = 0;
		}
		else {
			time += 0.016666f;
		}
	}
	// HPゲージに現在のHPを設定
	mpHPGauge->SetValue(mStatus.hp);
	// SPゲージに現在のSPを設定
	mpSPGauge->SetValue(mStatus.sp);
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