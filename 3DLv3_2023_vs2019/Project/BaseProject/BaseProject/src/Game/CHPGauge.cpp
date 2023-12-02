#include "CHPGauge.h"
#include "CImage.h"
#include "Maths.h"

// コンストラクタ
CHPGauge::CHPGauge()
{
	// HP設定
	mpHPFrameImage = new CImage(FRAME_IMAGE);
	mpHPFrameImage->SetSize(FRAME_SIZE_X, FRAME_SIZE_Y);
	mpHPBarImage = new CImage(GAUGE_BAR);
	mpHPBarImage->SetSize(BARSIZE_X, BARSIZE_Y);
}

// デストラクタ
CHPGauge::~CHPGauge()
{
}

// 更新
void CHPGauge::Update()
{
	// ゲージのフレームとバーの位置を設定
	mpHPFrameImage->SetPos(mPosition);
	mpHPBarImage->SetPos(mPosition + CVector2(FRAME_BORDER, FRAME_BORDER));

	// バーのサイズを最大値と現在値から求める
	CVector2 size = CVector2(BARSIZE_X, BARSIZE_Y);
	float percent = Math::Clamp01((float)mValue / mMaxValue);
	size.X(BARSIZE_X * percent);
	mpHPBarImage->SetSize(size);

	// HPバーの割合でバーの色を変更
	CColor color;
	// 10%以下は赤色
	if (percent <= 0.1f) color = CColor(1.0f, 0.0f, 0.0f);
	// 30%以下は黄色
	else if (percent <= 0.3f) color = CColor(1.0f, 1.0f, 0.0f);
	// それ以外は緑
	else color = CColor(0.0f, 1.0f, 0.0f);

	// バーに色を設定
	mpHPBarImage->SetColor(color);
}