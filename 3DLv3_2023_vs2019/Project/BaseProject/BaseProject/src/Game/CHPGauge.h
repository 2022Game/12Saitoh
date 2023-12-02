#pragma once
#include "CUIGauge.h"
class CImage;

// スタミナゲージ管理クラス
class CHPGauge : public CUIGauge
{
private:
	CImage* mpHPFrameImage;		// HPゲージのフレームのイメージ
	CImage* mpHPBarImage;		// HPゲージのバーイメージ

public:
	// コンストラクタ
	CHPGauge();
	// デストラクタ
	~CHPGauge();

	// 更新
	void Update() override;
};