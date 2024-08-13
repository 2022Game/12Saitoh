#include "CSword.h"
#include "CPlayer.h"
#include "CDragon.h"
#include "CColliderCapsule.h"

// コンストラクタ
CSword::CSword()
{
	mpSword = CResourceManager::Get<CModel>("Sword");

	mpSwordCollider = new CColliderCapsule(this, ELayer::eAttackCol,
		CVector(0.0f, 0.0f, 0.0f), CVector(0.0f, 0.0f, 16.0f), 1.5f);
	mpSwordCollider->SetCollisionLayers({ ELayer::eDamageCol });
	mpSwordCollider->SetCollisionTags({ ETag::eEnemy });

	// 最初は攻撃判定用のコライダーをオフにしておく
	mpSwordCollider->SetEnable(false);
}

// デストラクタ
CSword::~CSword()
{
	SAFE_DELETE(mpSwordCollider);
}

// 更新処理
void CSword::Update()
{
	// 納刀状態と抜刀状態で親のボーンを変更疑問
	if (CPlayer::Instance()->IsDrawn())
	{
		SetAttachMtx(CPlayer::Instance()->GetFrameMtx("Armature_weapon_r"));
	}
	else
	{
		SetAttachMtx(CPlayer::Instance()->GetFrameMtx("Armature_sword_holder"));
	}
}

// 衝突処理
void CSword::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	// 衝突した自分のコライダーが攻撃判定用のコライダーであれば
	if (self == mpSwordCollider)
	{
		CCharaBase* chara = dynamic_cast<CCharaBase*>(other->Owner());
		// 相手のコライダーの持ち主がキャラであれば
		if (chara != nullptr)
		{
			// 既に攻撃済みのキャラでなければ、
			if (!IsAttackHitObj(chara))
			{
				// ダメージ計算
				CPlayer* player = CPlayer::Instance();
				CDragon* dragon = CDragon::Instance();
				int atk = player->Status().atk;
				int def = dragon->Status().def;
				float motionvalue = player->GetMotionValue();
				int damage = dragon->TakeEnemyToDamage(atk, def, motionvalue, other->GetMeat());
				// ダメージを与える
				chara->TakeDamage(damage);

				// 闘気を増加する
				player->UpTouki();

				// 攻撃済みリストに追加
				AddAttackHitObj(chara);

				// 攻撃エフェクトを生成
				mpSwordEffect = new CNormalSwordEffect
				(
					hit.cross,
					CVector::zero,
					CQuaternion(0.0f, 0.0f, 0.0f).Matrix()
				);
			}
		}
	}
}

// 描画処理
void CSword::Render()
{
	mpSword->Render(Matrix());
}

// 武器の行列取得
CMatrix CSword::Matrix() const
{
	// 手に持っていない時は、自分自身の行列を返す
	if (mpAttachMtx == nullptr)
	{
		return CTransform::Matrix();
	}
	// 手に持っている時は、アタッチしている行列を返す
	else 
	{
		CMatrix sm;
		CMatrix rm;
		sm.Scale(12.0f, 12.0f, 12.0f);
		// 納刀状態と抜刀状態か判定し、描画の調整を行う
		if (CPlayer::Instance()->IsDrawn()) //抜刀
		{
			//rm.RotateZ(180.0f);
			rm = rm.RotateY(-90.0f);
			return rm * sm * (*mpAttachMtx);
		}
		else //納刀
		{
			rm.RotateX(-90.0f);
			return rm * sm * (*mpAttachMtx);
		}
	}
}

// 攻撃開始
void CSword::AttackStart()
{
	CWeapon::AttackStart();
	// 攻撃が始まったら、攻撃判定用のコライダーをオンにする
	mpSwordCollider->SetEnable(true);
}

// 攻撃終了
void CSword::AttackEnd()
{
	CWeapon::AttackEnd();
	// 攻撃が終われば、攻撃判定用のコライダーをオフにする
	mpSwordCollider->SetEnable(false);
}