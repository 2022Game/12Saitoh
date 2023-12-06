#include "CEnemy.h"
#include "CEffect.h"
#include "CCollisionManager.h"
#include "PlayerData.h"

CEnemy* CEnemy::spInstance = nullptr;

// コンストラクタ
CEnemy::CEnemy()
	: CXCharacter(ETag::eEnemy, ETaskPriority::eEnemy)
{
	// インスタンスの設定
	spInstance = this;

	// モデルデータ読み込み
	CModelX* model = CResourceManager::Get<CModelX>("Player");

	// CXCharacterの初期化
	Init(model);
	// ステータスの取得
	mStatus = ENEMY_STATUS[0];
}

//更新処理
void CEnemy::Update()
{
}

CEnemy* CEnemy::Instance()
{
	return spInstance;
}