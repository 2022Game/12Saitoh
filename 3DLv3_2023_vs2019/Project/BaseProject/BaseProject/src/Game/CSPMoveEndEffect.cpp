#include "CSPMoveEndEffect.h"

// アニメーションの1コマ表示時間
#define ANIM_TIME 0.06f
// エフェクトのサイズ
#define EFFECTSIZE 15.0f

// 通常攻撃エフェクトのアニメーションデータ
TexAnimData CSPMoveEndEffect::msAnimData = TexAnimData(4, 5, false, 19, ANIM_TIME);

// コンストラクタ
CSPMoveEndEffect::CSPMoveEndEffect(const CVector& pos)
	: CBillBoardImage(SPMOVE_END_EFFECTDATA, ETag::eEffect, ETaskPauseType::eGame)
	, mPos(pos)
{
	SetAnimData(&msAnimData);
	Position(mPos);
}

// デストラクタ
CSPMoveEndEffect::~CSPMoveEndEffect()
{
}

// 描画位置を取得
CVector CSPMoveEndEffect::HitPos()const
{
	return mPos;
}

// 更新処理
void CSPMoveEndEffect::Update()
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