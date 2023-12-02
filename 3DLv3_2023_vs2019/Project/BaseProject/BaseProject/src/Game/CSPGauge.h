#pragma once
#include "CUIGauge.h"
class CImage;

// スタミナゲージ管理クラス
class CSPGauge : public CUIGauge
{
private:
	CImage* mpSPFrameImage;		// SPゲージのフレームのイメージ
	CImage* mpSPBarImage;		// SPゲージのバーイメージ

public:
	// コンストラクタ
	CSPGauge();
	// デストラクタ
	~CSPGauge();

	// 更新
	void Update() override;
};