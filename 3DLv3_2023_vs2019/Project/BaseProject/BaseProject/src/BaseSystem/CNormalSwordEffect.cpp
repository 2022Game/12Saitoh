#include "CNormalSwordEffect.h"

// アニメーションの1コマ表示時間
#define ANIM_TIME 0.1f
// エフェクトのサイズ
#define EFFECTSIZE 8.0f

// 通常攻撃エフェクトのアニメーションデータ
TexAnimData CNormalSwordEffect::msAnimData = TexAnimData(1, 5, false, 5, ANIM_TIME);

// コンストラクタ
CNormalSwordEffect::CNormalSwordEffect(const CVector& hitpos)
	: CBillBoardImage(EFFECTDATA, ETag::eEffect , ETaskPauseType::eGame)
	, mHitPos(hitpos)
{
	SetAnimData(&msAnimData);
	Position(mHitPos);
}

// デストラクタ
CNormalSwordEffect::~CNormalSwordEffect()
{
}

// 描画位置を取得
CVector CNormalSwordEffect::HitPos()const
{
	return mHitPos;
}

// 更新処理
void CNormalSwordEffect::Update()
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