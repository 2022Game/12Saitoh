#pragma once
#pragma once
#include "CBillBoardImage.h"

#define COUNTER_EFFECTDATA2 "Effect/Counter2.png"

// 通常攻撃エフェクト
class CCounterEffect2 : public CBillBoardImage
{
public:
	// コンストラクタ
	CCounterEffect2(const CVector& hitpos);
	// デストラクタ
	~CCounterEffect2();

	// 描画位置を取得
	CVector HitPos()const;

	// 更新処理
	void Update() override;

private:

	// アニメーションデータ
	static TexAnimData msAnimData;
	CVector mHitPos;	// 衝突判定が起きた位置(エフェクトの表示座標)
};