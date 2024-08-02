#include "CNormalSwordEffect.h"

// アニメーションの1コマ表示時間
#define ANIM_TIME 0.1f
// エフェクトのサイズ
#define EFFECTSIZE 8.0f

// 通常攻撃エフェクトのアニメーションデータ
TexAnimData CNormalSwordEffect::msAnimData = TexAnimData(1, 5, false, 5, ANIM_TIME);

// コンストラクタ
CNormalSwordEffect::CNormalSwordEffect(const CVector& hitpos, const CVector& offsetPos, const CMatrix& offsetRot)
	: CBillBoardImage(EFFECTDATA, ETag::eEffect , ETaskPauseType::eGame)
	, mHitPos(hitpos)
	, mOffsetPos(offsetPos)
	, mOffsetRot(offsetRot)
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

// オフセット位置を設定
void CNormalSwordEffect::OffsetPos(const CVector& pos)
{
	mOffsetPos = pos;
}

// オフセット回転値を設定
void CNormalSwordEffect::OffsetRot(const CMatrix& rot)
{
	mOffsetRot = rot;
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