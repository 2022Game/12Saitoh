#pragma once
#include "CBillBoardImage.h"

#define SPMOVE_SWORD_EFFECTDATA "Effect/SPMoveEffect_Sword.png"

// 通常攻撃エフェクト
class CSPMoveEffect_Sword : public CBillBoardImage
{
public:
	// コンストラクタ
	CSPMoveEffect_Sword(const CVector& pos);
	// デストラクタ
	~CSPMoveEffect_Sword();

	// 描画位置を取得
	CVector HitPos()const;

	// 更新処理
	void Update() override;

private:

	// アニメーションデータ
	static TexAnimData msAnimData;
	CVector mPos;	// エフェクトの表示座標
};