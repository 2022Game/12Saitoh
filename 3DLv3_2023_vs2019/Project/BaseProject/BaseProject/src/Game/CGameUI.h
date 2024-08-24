#pragma once
#include"CUIBase.h"
#define TARGET_IMAGE "UI\\TargetUI.png"
#define SPMOVE_IMAGE "UI\\spmoveUI.png"
#define COUNTER_IMAGE "UI\\counterUI.png"
#define QKEY_IMAGE "UI\\Qkey.png"
#define CKEY_IMAGE "UI\\CKey.png"
#define LBUTTON_IMAGE "UI\\LButton.png"
#define RBUTTON_IMAGE "UI\\RButton.png"
#define USETOUKI_IMAGE "UI\\target.png"

class CImage;
class CHPGauge;
class CSPGauge;
class CToukiGauge;
class CHPGauge_Dragon;
// ゲーム中に表示するUIクラス
class CGameUI : public CUIBase
{
public:
	// コンストラクタ
	CGameUI();
	// デストラクタ
	~CGameUI();

	static void SetHP(int hp);
	static void SetMaxHP(int maxHp);
	static void SetSP(float sp);
	static void SetSPZeroFlag(bool flag);
	static void SetMaxSP(int maxSp);
	static void SetTouki(int touki);
	static void SetMaxTouki(int maxTouki);

	static void SetDragonHP(int hp);
	static void SetDragonMaxHP(int maxHP);
	static void SetShowDragonHPGauge(bool show);

	static void SetShowUI(bool show);

	static void SetColorSpMove(CColor color);
	static void SetColorCounter(CColor color);

	// 更新処理
	void Update() override;
	void Render() override;
private:
	static CGameUI* spInstance;

	CImage* mpTargetUI;
	CImage* mpQKeyUI;
	CImage* mpCKeyUI1;
	CImage* mpCKeyUI2;
	CImage* mpSPMoveUI;
	CImage* mpCounterUI;
	CImage* mpLButtonUI;
	CImage* mpRButtonUI;
	CImage* mpToukiUI1;
	CImage* mpToukiUI2;
	CImage* mpToukiUI3;

	CHPGauge* mpHPGauge;
	CSPGauge* mpSPGauge;
	CToukiGauge* mpToukiGauge;
	CHPGauge_Dragon* mpDragonHPGauge;
};