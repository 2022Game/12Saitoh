#pragma once
#include "CUIGauge.h"
class CImage;

// フレームの横のサイズ
#define DRAGON_FRAME_SIZE_X 485.0f * 1.7
// フレームの縦のサイズ
#define DRAGON_FRAME_SIZE_Y 30.0f * 1.3
// フレームの縁の幅
#define DRAGON_FRAME_BORDER 5.5f * 1.7
// バーの横のサイズ
#define DRAGON_BARSIZE_X (DRAGON_FRAME_SIZE_X - DRAGON_FRAME_BORDER * 1.9)
// バーの縦のサイズ
#define DRAGON_BARSIZE_Y (DRAGON_FRAME_SIZE_Y - DRAGON_FRAME_BORDER * 1.7)

// スタミナゲージ管理クラス
class CHPGauge_Dragon : public CUIGauge
{
private:
	CImage* mpHPFrameImage;	// HPゲージのフレームイメージ
	CImage* mpHPBarImage;	// HPゲージのバーイメージ

public:
	// コンストラクタ
	CHPGauge_Dragon();
	// デストラクタ
	~CHPGauge_Dragon();

	// 更新
	void Update() override;
};