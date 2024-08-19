#include "CCounterEffect2.h"

// アニメーションの1コマ表示時間
#define ANIM_TIME 0.06f
// エフェクトのサイズ
#define EFFECTSIZE 10.0f

// 通常攻撃エフェクトのアニメーションデータ
TexAnimData CCounterEffect2::msAnimData = TexAnimData(2, 5, false, 10, ANIM_TIME);

// コンストラクタ
CCounterEffect2::CCounterEffect2(const CVector& hitpos)
	: CBillBoardImage(COUNTER_EFFECTDATA2, ETag::eEffect, ETaskPauseType::eGame)
	, mHitPos(hitpos)
{
	SetAnimData(&msAnimData);
	Position(mHitPos);
}

// デストラクタ
CCounterEffect2::~CCounterEffect2()
{
}

// 描画位置を取得
CVector CCounterEffect2::HitPos()const
{
	return mHitPos;
}

// 更新処理
void CCounterEffect2::Update()
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