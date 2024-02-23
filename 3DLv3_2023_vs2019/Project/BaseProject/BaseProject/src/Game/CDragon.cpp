#include "CDragon.h"
#include "Maths.h"
#include "CPlayer.h"
#include "Primitive.h"
#include "CColliderSphere.h"
#include "CColliderLine.h"
#include "CDebugPrint.h"

#define GRAVITY			0.0625f	// 重力
#define ENEMY_HEIGHT	400.0f
#define FOV_LANGE		170.0f
#define FOV_ANGLE		60.0f

CDragon* CDragon::spInstance = nullptr;

// 敵のアニメーションデータテーブル
const CDragon::AnimData CDragon::ANIM_DATA[] =
{
	{ "Character\\Dragon\\anim\\idle1.x",			true,	102.0f,		0.0f },	// アイドル1
	{ "Character\\Dragon\\anim\\idle2.x",			true,	151.0f,		0.0f },	// アイドル2
	{ "Character\\Dragon\\anim\\land.x",			false,	200.0f,		0.0f },	// 着地
	{ "Character\\Dragon\\anim\\run.x",				true,	40.0f,		0.0f },	// 走り
	{ "Character\\Dragon\\anim\\walk.x",			true,	80.0f,		0.0f },	// 歩き
	{ "Character\\Dragon\\anim\\scream.x",			true,	202.0f,		0.0f },	// 咆哮
	{ "Character\\Dragon\\anim\\sleep.x",			true,	162.0f,		0.0f },	// 寝る
	{ "Character\\Dragon\\anim\\takeOff.x",			false,	121.0f,		0.0f },	// 離陸
	{ "Character\\Dragon\\anim\\attackflame.x",		false,	241.0f,		0.0f },	// ブレス攻撃
	{ "Character\\Dragon\\anim\\attackHand.x",		false,	182.0f,		0.0f },	// 飛び掛かり攻撃
	{ "Character\\Dragon\\anim\\attackMouth.x",		false,	86.0f,		0.0f },	// 噛みつき攻撃
	{ "Character\\Dragon\\anim\\die.x",				false,	260.0f,		0.0f },	// 死亡
	{ "Character\\Dragon\\anim\\flyFlame.x",		false,	182.0f,		0.0f },	// 空中ブレス攻撃
	{ "Character\\Dragon\\anim\\flyforward.x",		true,	80.0f,		0.0f },	// 空中前進
	{ "Character\\Dragon\\anim\\flyGlide.x",		true,	102.0f,		0.0f },	// 空中滑空
	{ "Character\\Dragon\\anim\\flyIdle.x",			true,	102.0f,		0.0f },	// 空中アイドル
	{ "Character\\Dragon\\anim\\getHit.x",			true,	86.0f,		0.0f },	// のけ反り
};

// コンストラクタ
CDragon::CDragon()
	: CXCharacter(ETag::eEnemy, ETaskPriority::eEnemy)
	, mMoveSpeed(CVector::zero)
	, mIsGrounded(true)
{
	// インスタンスの設定
	spInstance = this;

	// モデルデータ取得
	CModelX* model = CResourceManager::Get<CModelX>("Dragon");

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

	// 最初は待機のアニメーションを再生
	ChangeAnimation(EAnimType::eIdle1);

	// ランダム値を生成して取得した値によりステータスを設定
	int rand = Math::Rand(0.0f, 4.0f);
	mStatus = ENEMY_STATUS[rand];

	// 状態の設定
	mState = EState::eIdle;

	// 接地判定用の線分コライダの設定
	mpColliderLine = new CColliderLine
	(
		this, ELayer::eField,
		CVector(0.0f, 0.0f, 0.0f),
		CVector(0.0f, ENEMY_HEIGHT, 0.0f)
	);

	// コライダーの生成
	mpColliderSphere = new CColliderSphere(this, ELayer::eEnemy, 200.0f);
	mpColliderSphere->SetCollisionLayers({ ELayer::ePlayer });
	mpColliderSphere->SetCollisionTags({ ETag::ePlayer });

	mpDamageCol = new CColliderSphere(this, ELayer::eDamageCol, 500.0f);
	mpDamageCol->SetCollisionLayers({ ELayer::eAttackCol });
	//mpDamageCol->SetCollisionTags({ ETag::eSowrd });
}

// デストラクタ
CDragon::~CDragon()
{

}

// アニメーションの切り替え
void CDragon::ChangeAnimation(EAnimType type)
{
	if (!(EAnimType::None < type && type < EAnimType::Num)) return;
	AnimData data = ANIM_DATA[(int)type];
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
		// 移動状態
	case EState::eMove:
		break;
		// 攻撃状態
	case EState::eAttack:
		break;
		// 死亡状態
	case EState::eDeath:
		break;
	}

	// 重力の設定
	mMoveSpeed -= CVector(0.0f, GRAVITY, 0.0f);

	// 移動
	Position(Position() + mMoveSpeed);

	// 移動方向へ向ける
	CVector current = VectorZ();
	CVector target = mMoveSpeed;
	target.Y(0.0f);
	target.Normalize();
	CVector forward = CVector::Slerp(current, target, 0.0f);
	Rotation(CQuaternion::LookRotation(forward));

	CXCharacter::Update();

#ifdef _DEBUG
	// ドラゴンのステータスを表示
	CDebugPrint::Print("敵ステータス\n");
	CDebugPrint::Print("レベル : %d\nHP : %d\n攻撃力 : %d\n防御力 : %d",
		mStatus.level, mStatus.hp, mStatus.atk, mStatus.def);
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
}

// ダメージ処理
void CDragon::TakeDamage(int damage)
{
	mStatus.hp -= damage;
	if (mStatus.hp <= 0)
	{
		// 死亡処理
	}
	else
	{
		// のけ反りなどの被弾処理
	}
}

// 描画処理
void CDragon::Render()
{
	CXCharacter::Render();

	Primitive::DrawSector(
		Position() + CVector(0.0f, 1.0, 0.0f),
		-EulerAngles(),
		-FOV_ANGLE,
		FOV_ANGLE,
		FOV_LANGE,
		CColor::red,
		45
	);
}

// インスタンスを取得
CDragon* CDragon::Instance()
{
	return spInstance;
}