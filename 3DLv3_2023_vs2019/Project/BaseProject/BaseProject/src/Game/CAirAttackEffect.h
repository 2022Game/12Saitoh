#pragma once
#include "CBillBoardImage.h"

#define AIR_EFFECTDATA "Effect/AirAttack.png"

// 通常攻撃エフェクト
class CAirAttackEffect : public CBillBoardImage
{
public:
	// コンストラクタ
	CAirAttackEffect(const CVector& hitpos);
	// デストラクタ
	~CAirAttackEffect();

	// 描画位置を取得
	CVector HitPos()const;

	// 更新処理
	void Update() override;

private:

	// アニメーションデータ
	static TexAnimData msAnimData;
	CVector mHitPos;	// 衝突判定が起きた位置(エフェクトの表示座標)
};