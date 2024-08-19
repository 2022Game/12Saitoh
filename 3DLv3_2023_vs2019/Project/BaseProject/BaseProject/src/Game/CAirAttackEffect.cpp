#include "CAirAttackEffect.h"

// アニメーションの1コマ表示時間
#define ANIM_TIME 0.07f
// エフェクトのサイズ
#define EFFECTSIZE 7.0f

// 通常攻撃エフェクトのアニメーションデータ
TexAnimData CAirAttackEffect::msAnimData = TexAnimData(2, 5, false, 9, ANIM_TIME);

// コンストラクタ
CAirAttackEffect::CAirAttackEffect(const CVector& hitpos)
	: CBillBoardImage(AIR_EFFECTDATA, ETag::eEffect, ETaskPauseType::eGame)
	, mHitPos(hitpos)
{
	SetAnimData(&msAnimData);
	Position(mHitPos);
}

// デストラクタ
CAirAttackEffect::~CAirAttackEffect()
{
}

// 描画位置を取得
CVector CAirAttackEffect::HitPos()const
{
	return mHitPos;
}

// 更新処理
void CAirAttackEffect::Update()
{
	// 基底クラスの更新処理
	CBillBoardImage::Update();

	Scale(CVector::one * EFFECTSIZE);
	// アニメーションが終わったら、削除フラグを立てる
	if (IsEndAnim())
	{
		Kill();
	}
}