#include "CEnemy.h"
#include "CEffect.h"
#include "CCollisionManager.h"
#include "PlayerData.h"
#include "CColliderSphere.h"
#include "CPlayer.h"
#include "Primitive.h"
#define _USE_MATH_DEFINES
#include <math.h>

#define GRAVITY			0.0625f	// 重力
#define ENEMY_HEIGHT	3.0f
#define FOV_LANGE		100.0f
#define FOV_ANGLE		45.0f

CEnemy* CEnemy::spInstance = nullptr;

// 敵のアニメーションデータのテーブル
const CEnemy::AnimData CEnemy::ANIM_DATA[] =
{
	{ "",	true,	0.0f,	0.0f	},	// Tポーズ
	{ "",	true,	140.0f,	0.0f	},	// アイドル
	{ "",	false,	900.0f,	0.0f	},	// アイドル2
	{ "",	false,	300.0f,	0.0f	},	// アイドル3
	{ "",	true,	90.0f,	0.0f	},	// 歩き
	{ "",	true,	42.0f,	0.0f	},	// 走り
	{ "",	false,	66.0f,	0.0f	},	// 噛みつき
	{ "",	false,	138.0f,	0.0f	},	// 引っ掻き
	{ "",	false,	120.0f,	0.0f	},	// 尻尾攻撃	
	{ "",	false,	180.0f,	0.0f	},	// 咆哮
	{ "",	false,	45.0f,	0.0f	},	// 怯み(右)
	{ "",	false,	45.0f,	0.0f	},	// 怯み(左)
	{ "",	false,	180.0f,	0.0f	},	// 死亡
	{ "",	true,	135.0f,	0.0f	},	// 飛行
	{ "",	true,	45.0f,	0.0f	},	// 飛行始め
	{ "",	false,	183.0f,	0.0f	},	// 飛行中攻撃
	{ "",	false,	336.0f,	0.0f	},	// 飛行中ブレス
	{ "",	false,	63.0f,	0.0f	},	// 飛行中怯み
	{ "",	false,	183.0f,	0.0f	},	// 飛行中死亡
};

// コンストラクタ
CEnemy::CEnemy()
	: CXCharacter(ETag::eEnemy, ETaskPriority::eEnemy)
	, mElapsedTime(0.0f)
	, mIdleTime(0)
{
	// インスタンスの設定
	spInstance = this;

	// モデルデータ取得
	CModelX* model = CResourceManager::Get<CModelX>("Enemy");

	// アニメーションの分割
	model->SeparateAnimationSet(0, 0, 40, "Idle");				// アイドル1
	model->SeparateAnimationSet(0, 43, 343, "Idle2");			// アイドル2
	model->SeparateAnimationSet(0, 1004, 1104, "Idle3");		// アイドル3
	model->SeparateAnimationSet(0, 614, 644, "Walk");			// 歩き
	model->SeparateAnimationSet(0, 646, 660, "Run");			// 走り
	model->SeparateAnimationSet(0, 343, 365, "Attack_Bite");	// 噛みつき
	model->SeparateAnimationSet(0, 366, 412, "Attack_Scratching");// 引っ掻き
	model->SeparateAnimationSet(0, 476, 516, "Attack_Tail");	// 尻尾攻撃
	model->SeparateAnimationSet(0, 414, 474, "Roar");			// 咆哮
	model->SeparateAnimationSet(0, 518, 533, "Fear_Right");		// 怯み(右)
	model->SeparateAnimationSet(0, 535, 550, "Fear_Left");		// 怯み(左)
	model->SeparateAnimationSet(0, 552, 612, "Death");			// 死亡
	model->SeparateAnimationSet(0, 662, 707, "Fly");			// 飛行
	model->SeparateAnimationSet(0, 717, 730, "FlyStart");		// 飛行始め
	model->SeparateAnimationSet(0, 707, 768, "Fly_Attack");		// 飛行中攻撃
	model->SeparateAnimationSet(0, 771, 863, "Fly_Breath");		// 飛行中ブレス
	model->SeparateAnimationSet(0, 885, 906, "Fly_Fear");		// 飛行中怯み
	model->SeparateAnimationSet(0, 907, 968, "Fly_Death");		// 飛行中死亡


	// CXCharacterの初期化
	Init(model);

	// 最初は待機のアニメーションを再生
	ChangeAnimation(EAnimType::eIdle);
	// ステータスの取得
	mStatus = ENEMY_STATUS[0];

	// 状態の設定
	mState = EState::eIdle;

	// 線分コライダの設定
	mpColliderLine = new CColliderLine
	(
		this, ELayer::eField,
		CVector(0.0f, 0.0f, 0.0f),
		CVector(0.0f, ENEMY_HEIGHT, 0.0f)
	);
	// コライダーの生成
	mpHeadCol = new CColliderSphere(this, ELayer::eDamageCol, 0.2);
	mpHeadCol->SetCollisionLayers({ ELayer::eAttackCol });
	mpHeadCol->SetCollisionTags({ ETag::ePlayer });
	mpBodyCol = new CColliderSphere(this, ELayer::eDamageCol, 0.4);
	mpBodyCol->SetCollisionLayers({ ELayer::eAttackCol });
	mpBodyCol->SetCollisionTags({ ETag::ePlayer });

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

// プレイヤーを見つけたかどうか
bool CEnemy::IsFoundPlayer() const
{
	CVector playerPos = CPlayer::Instance()->Position();
	CVector enemyPos = Position();

	// 視野の角度の判定
	// 自身からプレイヤーまでのベクトルを取得
	CVector EP = (playerPos - enemyPos).Normalized();
	// 自身の正面方向のベクトルを取得
	CVector forward = VectorZ().Normalized();
	// 正面方向のベクトルとプレイヤーまでのベクトルの
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

//更新処理
void CEnemy::Update()
{
	// 状態に合わせて更新処理を切り替える
	switch (mState)
	{
		// アイドル状態
	case EState::eIdle:
		Update_Idle();
		break;
		// 移動状態
	case EState::eMove:
		Update_Move();
		break;
		// 攻撃状態
	case EState::eAttack:
		Update_Attack();
		break;
		// 死亡状態
	case EState::eDeath:
		Update_Death();
		break;
	}

	// 重力を設定
	mMoveSpeed -= CVector(0.0f, GRAVITY, 0.0f);

	// 移動
	Position(Position() + mMoveSpeed);

	// 移動方向へ向ける
	CVector current = VectorZ();
	CVector target = mMoveSpeed;
	target.Y(0.0f);
	target.Normalize();
	CVector forward = CVector::Slerp(current, target, 0.125f);
	Rotation(CQuaternion::LookRotation(forward));

	CXCharacter::Update();
	ColliderUpdate();

#ifdef _DEBUG
	CEnemy *enemydata = Instance();
	CVector pos = Position();
	CDebugPrint::Print("X : %1f, Y : %1f, Z : %1f\n", pos.X(), pos.Y(), pos.Z());

	// 敵の状態を切り替える
	if (CInput::PushKey('1')) mState = EState::eIdle;
	else if (CInput::PushKey('2')) mState = EState::eMove;
	else if (CInput::PushKey('3')) mState = EState::eAttack;
	else if (CInput::PushKey('4')) mState = EState::eDeath;
#endif
}

// 死亡処理
void CEnemy::Update_Death()
{

}
// 衝突処理
void CEnemy::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
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

	// 視野角の描画
	Primitive::DrawSector
	(
		Position() + CVector(0.0f, 1.5f, 0.0f),
		EulerAngles(),
		-FOV_ANGLE,
		FOV_ANGLE,
		FOV_LANGE,
		CColor::red,
		45
	);
}

CEnemy* CEnemy::Instance()
{
	return spInstance;
}