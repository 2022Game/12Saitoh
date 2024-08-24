#include "CHPGauge.h"
#include "CImage.h"
#include "Maths.h"
#include "CPlayer.h"

// コンストラクタ
CHPGauge::CHPGauge()
{
	// HP設定
	// フレーム
	mpHPFrameImage = new CImage
	(
		FRAME_IMAGE_PLAYER,
		ETaskPriority::eUI, 0, ETaskPauseType::eGame, 
		false, false
	);
	mpHPFrameImage->SetSize(FRAME_SIZE_X, FRAME_SIZE_Y);
	// 緑ゲージバー
	mpHPBarImage = new CImage
	(
		GAUGE_BAR_PLAYER,
		ETaskPriority::eUI, 0, ETaskPauseType::eGame,
		false, false
	);
	mpHPBarImage->SetSize(BARSIZE_X, BARSIZE_Y);
	// 赤ゲージバー
	mpRedBarImage = new CImage
	(
		GAUGE_BAR_PLAYER,
		ETaskPriority::eUI, 0, ETaskPauseType::eGame,
		false, false
	);
	mpRedBarImage->SetSize(BARSIZE_X, BARSIZE_Y);

}

// デストラクタ
CHPGauge::~CHPGauge()
{
	SAFE_DELETE(mpHPFrameImage);
	SAFE_DELETE(mpHPBarImage);
	SAFE_DELETE(mpRedBarImage);
}

void CHPGauge::Update_RedBar()
{
	// 赤ゲージの設定
	mpRedBarImage->SetPos(
		CVector2(mPosition.X() + mpHPBarImage->GetSize().X() + FRAME_BORDER,
			mPosition.Y() + FRAME_BORDER));

	// 暫定ダメージから赤ゲージのバーサイズを変更
	int value = CPlayer::Instance()->GetTemporaryDamage();
	CVector2 size = CVector2(BARSIZE_X, BARSIZE_Y);
	float percent = Math::Clamp01((float)value / mMaxValue);
	size.X(BARSIZE_X * percent);
	mpRedBarImage->SetSize(size);

	// 色の設定
	CColor color;
	// 赤色に設定
	color = CColor(1.0f, 0.0f, 0.0f);
	mpRedBarImage->SetColor(color);
}

// 更新
void CHPGauge::Update()
{
	// ゲージのフレームとバーの位置を設定
	mpHPFrameImage->SetPos(mPosition);
	mpHPBarImage->SetPos(mPosition + CVector2(FRAME_BORDER, FRAME_BORDER));

	// 緑ゲージのバーのサイズを最大値と現在値から求める
	CVector2 size = CVector2(BARSIZE_X, BARSIZE_Y);
	float percent = Math::Clamp01((float)mValue / mMaxValue);
	size.X(BARSIZE_X * percent);
	mpHPBarImage->SetSize(size);

	// 赤ゲージの更新を行う
	Update_RedBar();

	// HPバーの割合でバーの色を変更
	CColor color;
	// 緑色に設定
	color = CColor(0.0f, 1.0f, 0.0f);

	// バーに色を設定
	mpHPBarImage->SetColor(color);

	mpHPFrameImage->Update();
	mpHPBarImage->Update();
	mpRedBarImage->Update();
}

void CHPGauge::Render()
{
	mpHPFrameImage->Render();
	mpHPBarImage->Render();
	mpRedBarImage->Render();
}