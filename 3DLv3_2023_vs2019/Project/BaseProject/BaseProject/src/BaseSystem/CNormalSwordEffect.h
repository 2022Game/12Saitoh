#pragma once
#include "CBillBoardImage.h"

#define EFFECTDATA "Effect/NormalAttack.png"

// 通常攻撃エフェクト
class CNormalSwordEffect : public CBillBoardImage
{
public:
	// コンストラクタ
	CNormalSwordEffect(const CVector& hitpos, const CVector& offsetPos, const CMatrix& offsetRot);
	// デストラクタ
	~CNormalSwordEffect();

	// 描画位置を取得
	CVector HitPos()const;
	// オフセット座標を設定
	void OffsetPos(const CVector& pos);
	// オフセット回転値を設定
	void OffsetRot(const CMatrix& rot);

	// 更新処理
	void Update() override;

private:

	// アニメーションデータ
	static TexAnimData msAnimData;
	CVector mHitPos;	// 衝突判定が起きた位置(エフェクトの表示座標)
	CVector mOffsetPos;	// オフセット位置
	CMatrix mOffsetRot;	// オフセット回転値
};