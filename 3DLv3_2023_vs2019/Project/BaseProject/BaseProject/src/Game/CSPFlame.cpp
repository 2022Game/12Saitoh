#include "CSPFlame.h"
#include "Easing.h"
#include "CCharaBase.h"
#include "CPlayer.h"
#include "CDragon.h"

// 炎のスケール値の最大値
#define FLAME_SCALE 20.0f
// 炎のスケール値が最大値になるまでの時間
#define FLAME_SCALE_ANIM_TIME 2.0f
// アニメーションの1コマ表示時間
#define ANIM_TIME 0.0525f
// 炎用の重力
#define FLAME_GRAVITY 550.0f

// 炎のエフェクトのアニメーションデータ
TexAnimData CSPFlame::msAnimData = TexAnimData(8, 8, false, 64, ANIM_TIME);
// コンストラクタ
CSPFlame::CSPFlame(ETag tag)
	: CBillBoardImage("Effect/flame.png", tag, ETaskPauseType::eGame)
	, mMoveSpeed(CVector::zero)
	, mElapsedTime(0.0f)
	, mIsDeath(false)
{
	SetAnimData(&msAnimData);

	mpCollider = new CColliderSphere
	(
		this,
		ELayer::eAttackCol,
		1.0f
	);
	mpCollider->SetCollisionTags({ ETag::eField, ETag::eRideableObject, ETag::ePlayer });
	mpCollider->SetCollisionLayers({ ELayer::eField, ELayer::eDamageCol });
}

// デストラクタ
CSPFlame::~CSPFlame()
{
	SAFE_DELETE(mpCollider);
}

// 各パラメータを設定
void CSPFlame::Setup(const CVector& pos, const CVector& dir, float speed)
{
	Position(pos);
	mMoveSpeed = dir.Normalized() * speed;
}

// 削除フラグが立っているかどうか
bool CSPFlame::IsDeath() const
{
	return mIsDeath;
}

// カラーを設定
void CSPFlame::SetColor(const CColor& color)
{
	mMaterial.Diffuse()[0] = color.R();
	mMaterial.Diffuse()[1] = color.G();
	mMaterial.Diffuse()[2] = color.B();
	mMaterial.Diffuse()[3] = color.A();
	CObjectBase::SetColor(color);
}

// ブレンドタイプを設定
void CSPFlame::SetBlendType(EBlend type)
{
	mMaterial.SetBlendType(type);
}

// 衝突処理
void CSPFlame::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	if (other->Layer() == ELayer::eField)
	{
		float length = mMoveSpeed.Length();
		CVector n = hit.adjust.Normalized();
		float d = CVector::Dot(n, mMoveSpeed);
		mMoveSpeed = (mMoveSpeed - n * d).Normalized() * length;
		Position(Position() + hit.adjust * hit.weight);
	}

	if (other->Layer() == ELayer::eDamageCol)
	{
		CCharaBase* chara = dynamic_cast<CCharaBase*>(other->Owner());
		// 相手のコライダーの持ち主がキャラであれば
		if (chara != nullptr)
		{
			// ダメージを与える
			CPlayer* player = CPlayer::Instance();
			CDragon* dragon = CDragon::Instance();
			int atk = dragon->Status().atk;
			int def = player->Status().def;
			float motionvalue = dragon->GetMotionValue();
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

// 更新
void CSPFlame::Update()
{
	// 基底クラスの更新処理
	CBillBoardImage::Update();

	// 重力を掛けることで炎が地面を沿って移動するようにする
	mMoveSpeed.Y(mMoveSpeed.Y() - FLAME_GRAVITY * Time::DeltaTime());

	// 炎のエフェクトを移動
	CVector move = mMoveSpeed * Time::DeltaTime();
	Position(Position() + move);

	// スケール変更時間を経過していない
	if (mElapsedTime < FLAME_SCALE_ANIM_TIME)
	{
		// 経過時間に合わせて、徐々に炎を大きくする
		float per = mElapsedTime / FLAME_SCALE_ANIM_TIME;
		if (per < 1.0f)
		{
			float scale = Easing::QuadOut(per, 1.0f, 0.0f, 1.0f);
			Scale(CVector::one * scale * FLAME_SCALE);
		}
		else
		{
			Scale(CVector::one * FLAME_SCALE);
		}

		mElapsedTime += Time::DeltaTime();
	}
	// 移動時間が経過したら、削除する
	else
	{
		Scale(CVector::one * FLAME_SCALE);
	}

	// アニメーションが終わったら、削除フラグを立てる
	if (IsEndAnim())
	{
		mIsDeath = true;
	}
}

// 攻撃がヒットしたオブジェクトを追加
void CSPFlame::AddAttackHitObj(CObjectBase* obj)
{
	mAttackHitObjects.push_back(obj);
}

// 既に攻撃がヒットしているオブジェクトかどうか
bool CSPFlame::IsAttackHitObj(CObjectBase* obj) const
{
	// 既にリストに追加しているかを確認する
	auto find = std::find
	(
		mAttackHitObjects.begin(),
		mAttackHitObjects.end(),
		obj
	);
	return find != mAttackHitObjects.end();
}