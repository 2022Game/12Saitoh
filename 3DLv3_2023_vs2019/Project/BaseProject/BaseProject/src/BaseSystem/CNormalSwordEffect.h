#pragma once
#include "CBillBoardImage.h"

#define EFFECTDATA "Effect/NormalAttack.png"

// 通常攻撃エフェクト
class CNormalSwordEffect : public CBillBoardImage
{
public:
	// コンストラクタ
	CNormalSwordEffect(const CVector& hitpos);
	// デストラクタ
	~CNormalSwordEffect();

	// 描画位置を取得
	CVector HitPos()const;

	// 更新処理
	void Update() override;

private:

	// アニメーションデータ
	static TexAnimData msAnimData;
	CVector mHitPos;	// 衝突判定が起きた位置(エフェクトの表示座標)
};