#include "CToukiGauge.h"
#include "CImage.h"
#include "Maths.h"

// コンストラクタ
CToukiGauge::CToukiGauge()
{	
	// フレーム
	mpFrameImage = new CImage(FRAME_IMAGE_PLAYER);
	mpFrameImage->SetSize(TOUKI_FRAME_SIZE_X, TOUKI_FRAME_SIZE_Y);
	// ゲージバー
	mpBarImage = new CImage(GAUGE_BAR_PLAYER);
	mpBarImage->SetSize(TOUKI_BARSIZE_X, TOUKI_BARSIZE_Y);
}

// デストラクタ
CToukiGauge::~CToukiGauge()
{
	mpFrameImage->Kill();
	mpBarImage->Kill();
}

// 更新処理
void CToukiGauge::Update()
{
	// ゲージのフレームとバーの位置を設定
	mpFrameImage->SetPos(mPosition);
	mpBarImage->SetPos(mPosition + CVector2(TOUKI_FRAME_BORDER, TOUKI_FRAME_BORDER));

	// ゲージのバーのサイズを最大値と現在値から求める
	CVector2 size = CVector2(TOUKI_BARSIZE_X, TOUKI_BARSIZE_Y);
	float percent = Math::Clamp01((float)mValue / mMaxValue);
	size.X(TOUKI_BARSIZE_X * percent);
	mpBarImage->SetSize(size);

	// HPバーの割合でバーの色を変更
	CColor color;
	// 青色に設定
	color = CColor::blue;

	// バーに色を設定
	mpBarImage->SetColor(color);
}