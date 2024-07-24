#pragma once
#include "CUIGauge.h"

#define TOUKI_FRAME_SIZE_X 256.0f
// フレームの縦のサイズ
#define TOUKI_FRAME_SIZE_Y 20.0f
// フレームの縁の幅
#define TOUKI_FRAME_BORDER 3.2f
// バーの横のサイズ
#define TOUKI_BARSIZE_X (TOUKI_FRAME_SIZE_X - TOUKI_FRAME_BORDER * 1.6f)
// バーの縦のサイズ
#define TOUKI_BARSIZE_Y (TOUKI_FRAME_SIZE_Y - TOUKI_FRAME_BORDER * 1.6f)

// 闘気ゲージ管理クラス
class CToukiGauge : public CUIGauge
{
private:
	CImage* mpFrameImage;	// HPゲージのフレームイメージ
	CImage* mpBarImage;		// HPゲージのバーイメージ

public:
	// コンストラクタ
	CToukiGauge();
	// デストラクタ
	~CToukiGauge();

	// 更新
	void Update() override;
};