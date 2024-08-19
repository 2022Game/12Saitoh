#include "CSPMoveEffect_Sword.h"

// アニメーションの1コマ表示時間
#define ANIM_TIME 0.07f
// エフェクトのサイズ
#define EFFECTSIZE 10.0f

// 通常攻撃エフェクトのアニメーションデータ
TexAnimData CSPMoveEffect_Sword::msAnimData = TexAnimData(3, 5, false, 14, ANIM_TIME);

// コンストラクタ
CSPMoveEffect_Sword::CSPMoveEffect_Sword(const CVector& pos)
	: CBillBoardImage(SPMOVE_SWORD_EFFECTDATA, ETag::eEffect, ETaskPauseType::eGame)
	, mPos(pos)
{
	SetAnimData(&msAnimData);
	Position(mPos);
}

// デストラクタ
CSPMoveEffect_Sword::~CSPMoveEffect_Sword()
{
}

// 描画位置を取得
CVector CSPMoveEffect_Sword::HitPos()const
{
	return mPos;
}

// 更新処理
void CSPMoveEffect_Sword::Update()
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