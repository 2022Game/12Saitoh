#include "CEnemy.h"
#include "CEffect.h"
#include "CCollisionManager.h"
#include "PlayerData.h"

CEnemy* CEnemy::spInstance = nullptr;


// 敵のアニメーションデータのテーブル
const CEnemy::AnimData CEnemy::ANIM_DATA[] =
{
	{ "",									true,	0.0f	},	// Tポーズ
	{ "Character\\Enemy\\anim\\Idle.x",		true,	334.0f	},	// アイドル
};

// コンストラクタ
CEnemy::CEnemy()
	: CXCharacter(ETag::eEnemy, ETaskPriority::eEnemy)
{
	// インスタンスの設定
	spInstance = this;

	// モデルデータ取得
	CModelX* model = CResourceManager::Get<CModelX>("Enemy");

	// テーブル内のアニメーションデータを読み込み
	//int size = ARRAY_SIZE(ANIM_DATA);
	//for (int i = 0; i < size; i++)
	//{
	//	const AnimData& data = ANIM_DATA[i];
	//	if (data.path.empty()) continue;
	//	model->AddAnimationSet(data.path.c_str());
	//}

	// CXCharacterの初期化
	Init(model);

	// 最初は待機のアニメーションを再生
	//ChangeAnimation(EAnimType::eIdle);
	// ステータスの取得
	mStatus = ENEMY_STATUS[0];
}

// アニメーションの切り替え
void CEnemy::ChangeAnimation(EAnimType type)
{
	if (!(EAnimType::None < type && type < EAnimType::Num)) return;
	AnimData data = ANIM_DATA[(int)type];
	CXCharacter::ChangeAnimation((int)type, data.loop, data.frameLength, 0.0f);
}

//更新処理
void CEnemy::Update()
{
	CXCharacter::Update();

#ifdef _DEBUG
	auto enemydata = Instance();
	CVector enemy = Position();
	CDebugPrint::Print("X : %1f, Y : %1f, Z : %1f", enemy.X(), enemy.Y(), enemy.Z());
#endif
}

// 描画処理
void CEnemy::Render()
{
	CXCharacter::Render();
}

CEnemy* CEnemy::Instance()
{
	return spInstance;
}