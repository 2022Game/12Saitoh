#pragma once
#include "CUIBase.h"
class CImage;

// HP・SPフレーム画像のファイルパス
#define FRAME_IMAGE_PLAYER "UI\\Gauge_Frame.png"
// HP・SPゲージバー画像のファイルパス
#define GAUGE_BAR_PLAYER "UI\\GaugeBar.png"

// ドラゴンのHPフレーム画像のファイルパス
#define FRAME_IMAGE_DRAGON "UI\\Dragon_HP_Frame.png"
// ドラゴンんのHPバー画像のファイルパス
#define GAUGE_BAR_DRAGON "UI\\Dragon_HP_Gauge.png"

// フレームの横のサイズ
#define FRAME_SIZE_X (256.0f) * 1.7
// フレームの縦のサイズ
#define FRAME_SIZE_Y 20.0f
// フレームの縁の幅
#define FRAME_BORDER 4.2f
// バーの横のサイズ
#define BARSIZE_X (FRAME_SIZE_X - FRAME_BORDER * 1.9)
// バーの縦のサイズ
#define BARSIZE_Y (FRAME_SIZE_Y - FRAME_BORDER * 1.6f)

// UIのゲージ表示クラス
class CUIGauge : public CUIBase
{
public:
	// コンストラクタ
	CUIGauge();
	// デストラクタ
	~CUIGauge();

	// 最大値を設定
	void SetMaxValue(int value);
	// 現在値を設定
	void SetValue(int value);
	// 現在値を設定
	void SetValueF(float valuef);

	// 更新
	void Update() override;

protected:
	int mMaxValue;			// ポイントの最大値
	int mValue;				// ポイントの現在値
	float mValueF;			// ポイントの現在値(float型)
};