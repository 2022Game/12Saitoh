#pragma once
#include "CBillBoardImage.h"

#define COUNTER_EFFECTDATA1 "Effect/Counter1.png"

// 通常攻撃エフェクト
class CCounterEffect : public CBillBoardImage
{
public:
	// コンストラクタ
	CCounterEffect(const CVector& hitpos);
	// デストラクタ
	~CCounterEffect();

	// 描画位置を取得
	CVector HitPos()const;

	// 更新処理
	void Update() override;

private:

	// アニメーションデータ
	static TexAnimData msAnimData;
	CVector mHitPos;	// 衝突判定が起きた位置(エフェクトの表示座標)
};