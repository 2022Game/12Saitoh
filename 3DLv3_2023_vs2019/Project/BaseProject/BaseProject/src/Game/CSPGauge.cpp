#include "CSPGauge.h"
#include "CImage.h"
#include "Maths.h"

// コンストラクタ
CSPGauge::CSPGauge()
	: mZeroFlag(false)
{
	// SP設定
	mpSPFrameImage = new CImage(FRAME_IMAGE_PLAYER);
	mpSPFrameImage->SetSize(FRAME_SIZE_X, FRAME_SIZE_Y);
	mpSPBarImage = new CImage(GAUGE_BAR_PLAYER);
	mpSPBarImage->SetSize(BARSIZE_X, BARSIZE_Y);
}

// デストラクタ
CSPGauge::~CSPGauge()
{
	mpSPFrameImage->Kill();
	mpSPBarImage->Kill();
}

// ダッシュ時のスタミナ回復中のフラグを取得
void CSPGauge::SetSPZeroFlag(bool flag)
{
	mZeroFlag = flag;
}

// 更新
void CSPGauge::Update()
{
	// ゲージのフレームとバーの位置を設定
	mpSPFrameImage->SetPos(mPosition);
	mpSPBarImage->SetPos(mPosition + CVector2(FRAME_BORDER, FRAME_BORDER));

	// バーのサイズを最大値と現在値から求める
	CVector2 size = CVector2(BARSIZE_X, BARSIZE_Y);
	float percent = Math::Clamp01(mValueF / mMaxValue);
	size.X(BARSIZE_X * percent);
	mpSPBarImage->SetSize(size);

	// SPバーの割合でバーの色を変更
	CColor color;
	// 回復時
	if (mZeroFlag)
	{
		// 回復中は薄い赤色で描画
		color = CColor(0.9, 0.4f, 0.0f);
	}
	// 通常時
	else
	{
		// 10%以下は赤色
		if (percent <= 0.3f) color = CColor(1.0f, 0.0f, 0.0f);
		// それ以外は黄色
		else color = CColor(1.0f, 1.0f, 0.0f);
	}
	// バーに色を設定
	mpSPBarImage->SetColor(color);
}