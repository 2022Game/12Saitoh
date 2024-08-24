#pragma once
#include "CUIGauge.h"
class CImage;

// スタミナゲージ管理クラス
class CSPGauge : public CUIGauge
{
private:
	CImage* mpSPFrameImage;		// SPゲージのフレームのイメージ
	CImage* mpSPBarImage;		// SPゲージのバーイメージ

	bool mZeroFlag; // 回復中のフラグ
public:
	// コンストラクタ
	CSPGauge();
	// デストラクタ
	~CSPGauge();

	// ダッシュ時のスタミナ回復中のフラグを取得
	void SetSPZeroFlag(bool flag);

	// 更新
	void Update() override;
	void Render() override;
};