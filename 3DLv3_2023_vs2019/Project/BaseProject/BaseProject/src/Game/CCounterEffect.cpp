#include "CCounterEffect.h"

// アニメーションの1コマ表示時間
#define ANIM_TIME 0.07f
// エフェクトのサイズ
#define EFFECTSIZE 8.0f

// 通常攻撃エフェクトのアニメーションデータ
TexAnimData CCounterEffect::msAnimData = TexAnimData(2, 5, false, 9, ANIM_TIME);

// コンストラクタ
CCounterEffect::CCounterEffect(const CVector& hitpos)
	: CBillBoardImage(COUNTER_EFFECTDATA1, ETag::eEffect, ETaskPauseType::eGame)
	, mHitPos(hitpos)
{
	SetAnimData(&msAnimData);
	Position(mHitPos);
}

// デストラクタ
CCounterEffect::~CCounterEffect()
{
}

// 描画位置を取得
CVector CCounterEffect::HitPos()const
{
	return mHitPos;
}

// 更新処理
void CCounterEffect::Update()
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