#pragma once
#include "CUIGauge.h"
class CImage;

// スタミナゲージ管理クラス
class CHPGauge : public CUIGauge
{
private:
	CImage* mpHPFrameImage;	// HPゲージのフレームのイメージ
	CImage* mpHPBarImage;	// HPゲージのバーイメージ
	CImage* mpRedBarImage;	// 赤ゲージのバーイメージ

public:
	// コンストラクタ
	CHPGauge();
	// デストラクタ
	~CHPGauge();

	// 赤ゲージの更新処理
	void Update_RedBar();

	// 更新
	void Update() override;
	void Render() override;
};