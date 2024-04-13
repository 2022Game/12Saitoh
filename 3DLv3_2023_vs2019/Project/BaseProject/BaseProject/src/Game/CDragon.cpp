#include "CDragon.h"
#include "Maths.h"
#include "CPlayer.h"
#include "Primitive.h"
#include "CColliderSphere.h"
#include "CColliderLine.h"
#include "CDebugPrint.h"
#include "CFlamethrower.h"
#include "Global.h"

CDragon* CDragon::spInstance = nullptr;

// コンストラクタ
CDragon::CDragon()
	: CXCharacter(ETag::eEnemy, ETaskPriority::eEnemy)
	, mMoveSpeed(CVector::zero)
	, mIsGrounded(true)
	, mIsAngry(false)
	, mAngryStandardValue(0)
	, mAngryValue(0)
	, mRandSave(0)
	, mBatteleStep(0)
	, mElapsedTime(0.0f)
	, mAngryElapsedTime(0.0f)
	, mChaseElapsedTime(0.0f)
{
	// インスタンスの設定
	spInstance = this;

	// モデルデータ取得
	CModelX* model = CResourceManager::Get<CModelX>("Dragon");

	// テーブル内のアニメーションデータを読み込み
	int size = DragonData::GetAnimDataSize();
	for (int i = 0; i < size; i++)
	{
		const DragonData::AnimData& data = DragonData::GetAnimData(i);
		if (data.path.empty()) continue;
		model->AddAnimationSet(data.path.c_str());
	}
	// CXCharacterの初期化
	Init(model);

	// 最初は待機のアニメーションを再生
	ChangeAnimation(EDragonAnimType::eIdle1);

	// ランダム値を生成して取得した値によりステータスを設定
	int rand = Math::Rand(0.0f, 5.0f);
	mStatus = ENEMY_STATUS[rand];
	mMaxStatus = mStatus;

	// 怒り値の最大値を設定
	// 最大値は最大HPの１割を基準として設定
	mAngryStandardValue = mMaxStatus.hp / 10;

	// 状態の設定
	mState = EState::eIdle;

	// 接地判定用の線分コライダの設定
	mpColliderLine = new CColliderLine
	(
		this, ELayer::eField,
		CVector(0.0f, 0.0f, 0.0f),
		CVector(0.0f, ENEMY_HEIGHT, 0.0f)
	);
	mpColliderLine->SetCollisionLayers({ ELayer::eField });
	// 壁との当たり判定用コライダの設定
	mpColliderLine2 = new CColliderLine
	(
		this, ELayer::eField,
		CVector(-150.0f, 200.0f, 0.0f),
		CVector(150.0f, 200.0f, 0.0f)
	);
	mpColliderLine2->SetCollisionLayers({ ELayer::eField });
	mpColliderLine3 = new CColliderLine
	(
		this, ELayer::eField,
		CVector(0.0f, 200.0f, -300.0f),
		CVector(0.0f, 200.0f, 500.0f)
	);
	mpColliderLine3->SetCollisionLayers({ ELayer::eField });

	// コライダーの生成
	// プレイヤーとの押し戻し用コライダー
	mpBodyCol = new CColliderSphere(this, ELayer::eEnemy, 200.0f);
	mpBodyCol->SetCollisionLayers({ ELayer::ePlayer });
	mpBodyCol->SetCollisionTags({ ETag::ePlayer });

	// ダメージを受ける用のコライダー
	mpDamageCol = new CColliderSphere(this, ELayer::eDamageCol, 500.0f);
	mpDamageCol->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageCol->SetCollisionTags({ ETag::eWeapon });
	mpDamageCol->SetMeat(100);

	// 攻撃用のコライダー
	mpAttackMouthCol = new CColliderSphere(this, ELayer::eAttackCol, 1.5f);
	mpAttackMouthCol->SetCollisionLayers({ ELayer::eDamageCol });
	mpAttackMouthCol->SetCollisionTags({ ETag::ePlayer });
	// コライダーをアタッチ
	const CMatrix* attackcol = GetFrameMtx("Armature_Tongue02");
	mpAttackMouthCol->SetAttachMtx(attackcol);

	// 最初は攻撃判定用のコライダーはオフにしておく
	mpAttackMouthCol->SetEnable(false);

	const CMatrix* flamemtx = GetFrameMtx("Armature_UpperMouth01");
	mpFlamethrower = new CFlamethrower
	(
		this, flamemtx,
		CVector(0.0f, -20.0f, 0.0f),
		CQuaternion(0.0f, 90.0f,0.0f).Matrix()
	);
}

// デストラクタ
CDragon::~CDragon()
{
	SAFE_DELETE(mpColliderLine);
	SAFE_DELETE(mpBodyCol);
	SAFE_DELETE(mpDamageCol);
	SAFE_DELETE(mpAttackMouthCol);
}

// インスタンスを取得
CDragon* CDragon::Instance()
{
	return spInstance;
}

// アニメーションの切り替え
void CDragon::ChangeAnimation(EDragonAnimType type)
{
	if (!(EDragonAnimType::None < type && type < EDragonAnimType::Num)) return;
	DragonData::AnimData data = DragonData::GetAnimData((int)type);
	CXCharacter::ChangeAnimation((int)type, data.loop, data.frameLength, data.motionValue);
}

// プレイヤーを見つけたかどうか
bool CDragon::IsFoundPlayer() const
{
	CVector playerPos = CPlayer::Instance()->Position();
	CVector enemyPos = Position();

	// 視野角度の判定
	// 自身からプレイヤーまでのベクトルを取得
	CVector EP = (playerPos - enemyPos).Normalized();
	// 自身の正面方向のベクトルを取得
	CVector forward = VectorZ().Normalized();
	// 正面方向のベクトルとプレイヤーまでの
	// 内積から角度を求める
	float dotZ = forward.Dot(EP);

	// 求めた角度が視野角度外の場合、falseを返す
	if (dotZ <= cosf(FOV_ANGLE * M_PI / 180.0f)) return false;

	// 距離の判定
	// 自身からプレイヤーまでの距離を求める
	float distance = (playerPos - enemyPos).Length();
	// 求めた距離が視野距離よりも遠い場合、falseを返す
	if (distance > FOV_LANGE) return false;

	// 視野判定と距離判定を通ったのでtrueを返す
	return true;
}

// バックステップできるかどうか
bool CDragon::IsBackStep() const
{
	// レイの開始地点
	CVector startPos = Position() + CVector(0.0f,50.0f,0.0f);
	// レイの終了地点
	CVector endPos = -VectorZ().Normalized() * FIELD_RADIUS;

	float outDist = 0.0f;
	// フィールドとレイの当たり判定を行う
	gField->CollisionRay(startPos, endPos, &outDist);
	
	// バックステップをする範囲がある
	if (outDist > 150.0f) return true;

	return false;
}


//プレイヤーとの距離を取得
CDragon::EDistanceType CDragon::PlayerFromDistance()
{
	CVector playerPos = CPlayer::Instance()->Position();
	CVector enemyPos = Position();

	// 距離の判定
	float distance = (playerPos - enemyPos).Length();
	// 距離と判別用の値を比べて、距離を判別
	if (distance >= 280.0f) mDistanceType = EDistanceType::eFar;// 遠距離
	else if (distance >= 230.0f) mDistanceType = EDistanceType::eMedium;// 中距離
	else mDistanceType = EDistanceType::eNear;// 近距離

	// 判別した距離を返す
	return mDistanceType;
}

// 更新処理
void CDragon::Update()
{
	// 状態に合わせて更新処理を切り替える
	switch (mState)
	{
		// 待機状態
	case EState::eIdle:
		UpdateIdle();
		break;
		// 戦闘状態
	case EState::eBattle:
		UpdateBattle();
		break;
		// 死亡状態
	case EState::eDeath:
		break;
	}

	// 重力の設定
	mMoveSpeed -= CVector(0.0f, GRAVITY, 0.0f);

	// 移動
	Position(Position() + mMoveSpeed);

	// 攻撃中または、バックステップ以外の時は移動方向へ向ける
	if (mBatteleStep != 2 &&
		AnimationIndex() != (int)EDragonAnimType::eBackStep)
	{
		CVector current = VectorZ();
		CVector target = mMoveSpeed;
		target.Y(0.0f);
		target.Normalize();
		CVector forward = CVector::Slerp(current, target, 0.02f);
		Rotation(CQuaternion::LookRotation(forward));
	}

	CXCharacter::Update();
	mpAttackMouthCol->Update();

#ifdef _DEBUG
	// ドラゴンのステータスを表示
	CDebugPrint::Print("敵ステータス\n");
	CDebugPrint::Print("レベル : %d\nHP : %d\n攻撃力 : %d\n防御力 : %d\n",
		mStatus.level, mStatus.hp, mStatus.atk, mStatus.def);

	// プレイヤーとの距離を表示
	CVector pPos = CPlayer::Instance()->Position();
	CVector ePos = Position();
	float distance = (pPos - ePos).Length();
	CDebugPrint::Print("プレイヤーとの距離 : %.1f\n", distance);

	if (mDistanceType == EDistanceType::eNear) CDebugPrint::Print("近距離\n");
	else if (mDistanceType == EDistanceType::eMedium) CDebugPrint::Print("中距離\n");
	else CDebugPrint::Print("遠距離\n");

	if (CInput::PushKey('1'))
	{
		int p = GetHPPercent();
		printf("%d\n", p);
	}

	if (IsBackStep()) CDebugPrint::Print("true\n");
	else CDebugPrint::Print("false\n");

#endif
}

// 衝突処理
void CDragon::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	// 地面との当たり判定
	if (self == mpColliderLine)
	{
		if (other->Layer() == ELayer::eField)
		{
			mMoveSpeed.Y(0.0f);
			// めり込まないように調整
			Position(Position() + hit.adjust);
			mIsGrounded = true;
		}
	}
	// 壁との当たり判定
	if (self == mpColliderLine2)
	{
		if (other->Layer() == ELayer::eField)
		{
			mMoveSpeed.X(0.0f);
			// めり込まないように調整
			Position(Position() + hit.adjust);
		}
	}
	if (self == mpColliderLine3)
	{
		if (other->Layer() == ELayer::eField)
		{
			mMoveSpeed.Z(0.0f);
			// めり込まないように調整
			Position(Position() + hit.adjust);
		}
	}

	// 衝突したコライダーが攻撃判定用のコライダーであれば
	if (self == mpAttackMouthCol)
	{
		CCharaBase* chara = dynamic_cast<CCharaBase*>(other->Owner());
		// 相手のコライダーの持ち主がキャラであれば
		if (chara != nullptr)
		{
			// ダメージを与える
			CPlayer* player = CPlayer::Instance();
			int atk = Status().atk;
			int def = player->Status().def;
			float motionvalue = GetMotionValue();
			// ダメージ計算
			int damage = player->TakePlayerToDamage(atk, def, motionvalue);

			// 既に攻撃済みのキャラでなければ
			if (!IsAttackHitObj(chara))
			{
				// ダメージを与える
				chara->TakeDamage(damage);

				// 攻撃済みのリストに追加
				AddAttackHitObj(chara);
			}
		}
	}
}

// 描画処理
void CDragon::Render()
{
	CXCharacter::Render();
#ifdef _DEBUG

	// 視野判定用
	Primitive::DrawSector(
		Position() + CVector(0.0f, 1.0, 0.0f),
		-EulerAngles(),
		-FOV_ANGLE,
		FOV_ANGLE,
		FOV_LANGE,
		CColor::red,
		45
	);
#endif
}

// ダメージ処理
void CDragon::TakeDamage(int damage)
{
	// ダメージ分HPを減少
	mStatus.hp -= damage;

	// 怒り状態で無ければ、ダメージ分怒り値増加
	if (!mIsAngry)
	{
		mAngryValue += damage;
		// 怒り値が基準より大きくなった場合、怒り状態へ移行
		if (mAngryValue >= mAngryStandardValue)
		{
			mIsAngry = true;
			mAngryValue = mAngryStandardValue;
			ChangeAnimation(EDragonAnimType::eScream);
		}
	}
	// 怒り状態で有れば、ダメージの半分の値だけ怒り値を減少
	else
	{
		mAngryValue -= damage / 2;
		// 怒り値が0以下になった場合、怒り状態を解除
		if (mAngryValue <= 0)
		{
			mIsAngry = false;
			mAngryValue = 0;
		}
	}

	if (mStatus.hp <= 0)
	{
		// 死亡処理
	}
	else
	{
		// のけ反りなどの被弾処理
	}
}

// 攻撃開始
void CDragon::AttackStart()
{
	CCharaBase::AttackStart();
	// 攻撃が始まったら、攻撃判定用のコライダーをオンにする
	switch (AnimationIndex())
	{
	case (int)EDragonAnimType::eAttackMouth:// 噛みつき攻撃
		mpAttackMouthCol->SetEnable(true);
		break;
	case (int)EDragonAnimType::eAttackHand:// 飛び掛かり攻撃
		break;
	}
}

// 攻撃終了
void CDragon::AttackEnd()
{
	CCharaBase::AttackEnd();
	// 攻撃が終われば、攻撃判定用のコライダーをオフにする
	mpAttackMouthCol->SetEnable(false);
}