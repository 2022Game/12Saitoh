#include "CDragon.h"
#include "Maths.h"

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

// 更新処理
void CDragon::Update()
{
	CXCharacter::Update();
}

// 衝突処理
void CDragon::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{

}

// 描画処理
void CDragon::Render()
{
	CXCharacter::Render();
}

// インスタンスを取得
CDragon* CDragon::Instance()
{
	return spInstance;
}