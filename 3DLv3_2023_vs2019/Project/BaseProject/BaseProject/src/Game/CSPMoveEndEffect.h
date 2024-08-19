#pragma once
#include "CBillBoardImage.h"

#define SPMOVE_END_EFFECTDATA "Effect/SPMoveEndEffect1.png"

// 通常攻撃エフェクト
class CSPMoveEndEffect : public CBillBoardImage
{
public:
	// コンストラクタ
	CSPMoveEndEffect(const CVector& pos);
	// デストラクタ
	~CSPMoveEndEffect();

	// 描画位置を取得
	CVector HitPos()const;

	// 更新処理
	void Update() override;

private:

	// アニメーションデータ
	static TexAnimData msAnimData;
	CVector mPos;	// エフェクトの表示座標
};