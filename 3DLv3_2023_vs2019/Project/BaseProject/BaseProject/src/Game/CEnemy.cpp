#include "CEnemy.h"
#include "CEffect.h"
#include "CCollisionManager.h"
#include "PlayerData.h"
#include "CColliderSphere.h"
#include "CPlayer.h"

CEnemy* CEnemy::spInstance = nullptr;

// 敵のアニメーションデータのテーブル
const CEnemy::AnimData CEnemy::ANIM_DATA[] =
{
	{ "",	true,	0.0f,	0.0f	},	// Tポーズ
	{ "",	true,	334.0f,	0.0f	},	// アイドル
};

// コンストラクタ
CEnemy::CEnemy()
	: CXCharacter(ETag::eEnemy, ETaskPriority::eEnemy)
{
	// インスタンスの設定
	spInstance = this;

	// モデルデータ取得
	CModelX* model = CResourceManager::Get<CModelX>("Enemy");

	// アニメーションの分割
	model->SeparateAnimationSet(0, 0, 40, "Idle");

	// CXCharacterの初期化
	Init(model);

	// 最初は待機のアニメーションを再生
	ChangeAnimation(EAnimType::eIdle);
	// ステータスの取得
	mStatus = ENEMY_STATUS[0];

	mpHeadCol = new CColliderSphere(this, ELayer::eDamageCol, 0.2);
	mpHeadCol->SetCollisionLayers({ ELayer::eAttackCol });
	mpHeadCol->SetCollisionTags({ ETag::ePlayer });

	const CMatrix* headcol = GetFrameMtx("Bip01_Bip01_Head");
	mpHeadCol->SetAttachMtx(headcol);
}

// アニメーションの切り替え
void CEnemy::ChangeAnimation(EAnimType type)
{
	if (!(EAnimType::None < type && type < EAnimType::Num)) return;
	AnimData data = ANIM_DATA[(int)type];
	CXCharacter::ChangeAnimation((int)type, data.loop, data.frameLength, data.motionValue);
}

// コライダーの更新処理
void CEnemy::ColliderUpdate()
{
	mpHeadCol->Update();
}

//更新処理
void CEnemy::Update()
{
	CXCharacter::Update();
	ColliderUpdate();

#ifdef _DEBUG
	auto enemydata = Instance();
	CVector enemy = Position();
	CDebugPrint::Print("X : %1f, Y : %1f, Z : %1f\n", enemy.X(), enemy.Y(), enemy.Z());

#endif
}

// 衝突処理
void CEnemy::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
//	if (self == mpAttackCol)
//	{
//		CPlayer* player = dynamic_cast<CPlayer*>(other->Owner());
//		if (player != nullptr)
//		{
//			int atk = mStatus.atk;
//			int def = player->Status().def;
//			int motion = mMotionValue;
//			int damage = TakePlayerToDamage(atk, def, motion);
//			player->TakeDamage(damage);
//		}
//	}
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