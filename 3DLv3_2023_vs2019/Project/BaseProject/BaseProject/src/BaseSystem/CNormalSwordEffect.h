#pragma once
#include "CLineEffect.h"

// 通常攻撃エフェクト
class CNormalSwordEffect : public CLineEffect
{
public:
	// コンストラクタ
	CNormalSwordEffect(const CVector& start, const CVector& end);
	// デストラクタ
	~CNormalSwordEffect();

	// 更新処理
	void Update() override;
	// 描画処理
	void Render() override;
};