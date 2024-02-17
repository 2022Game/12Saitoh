#include "CDragon.h"

CDragon* CDragon::spInstance = nullptr;

// 敵のアニメーションデータテーブル
const CDragon::AnimData CDragon::ANIM_DATA[] =
{
	{"Charcter\\Dragon\\anim\\Idle1.x",			true,	40.0f,	0.0f},	// アイドル
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