#include "CHPGauge_Dragon.h"
#include "CImage.h"
#include "Maths.h"

// コンストラクタ
CHPGauge_Dragon::CHPGauge_Dragon()
{
	// HP設定
	mpHPFrameImage = new CImage(FRAME_IMAGE_DRAGON);
	mpHPFrameImage->SetSize(DRAGON_FRAME_SIZE_X, DRAGON_FRAME_SIZE_Y);
	// 緑ゲージバー
	mpHPBarImage = new CImage(GAUGE_BAR_DRAGON);
	mpHPBarImage->SetSize(DRAGON_BARSIZE_X, DRAGON_BARSIZE_Y);
}

// デストラクタ
CHPGauge_Dragon::~CHPGauge_Dragon()
{
	mpHPFrameImage->Kill();
	mpHPBarImage->Kill();
}

// 更新
void CHPGauge_Dragon::Update()
{
	// ゲージのフレームとバーの位置を設定
	mpHPFrameImage->SetPos(mPosition);
	mpHPBarImage->SetPos(mPosition + CVector2(DRAGON_FRAME_BORDER, DRAGON_FRAME_BORDER));

	// HPゲージのバーのサイズを最大値と現在値から求める
	CVector2 size = CVector2(DRAGON_BARSIZE_X, DRAGON_BARSIZE_Y);
	float percent = Math::Clamp01((float)mValue / mMaxValue);
	size.X(DRAGON_BARSIZE_X * percent);
	mpHPBarImage->SetSize(size);
}