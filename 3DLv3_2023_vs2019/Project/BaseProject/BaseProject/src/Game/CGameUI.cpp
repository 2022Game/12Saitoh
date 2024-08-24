#include "CGameUI.h"
#include "CImage.h"
#include "CHPGauge.h"
#include "CSPGauge.h"
#include "CToukiGauge.h"
#include "CHPGauge_Dragon.h"

CGameUI* CGameUI::spInstance = nullptr;

// コンストラクタ
CGameUI::CGameUI()
{
	spInstance = this;
	// ターゲットカメラUI
	mpTargetUI = new CImage
	(
		TARGET_IMAGE,
		ETaskPriority::eUI, 0, ETaskPauseType::eGame,
		false, false
	);
	mpTargetUI->SetPos(1200.0f, 20.0f);
	mpTargetUI->SetSize(50.0f, 50.0f);
	mpQKeyUI = new CImage
	(
		QKEY_IMAGE,
		ETaskPriority::eUI, 0, ETaskPauseType::eGame,
		false, false
	);
	mpQKeyUI->SetPos(1210.0f, 73.0f);
	mpQKeyUI->SetSize(30.0f, 35.0f);

	// 強闘技UI
	mpSPMoveUI = new CImage
	(
		SPMOVE_IMAGE,
		ETaskPriority::eUI, 0, ETaskPauseType::eGame,
		false, false
	);
	mpSPMoveUI->SetPos(1200.0f, 570.0f);
	mpSPMoveUI->SetSize(70.0f, 70.0f);
	mpCKeyUI1 = new CImage
	(
		CKEY_IMAGE,
		ETaskPriority::eUI, 0, ETaskPauseType::eGame,
		false, false
	);
	mpCKeyUI1->SetPos(1200.0f, 650.0f);
	mpCKeyUI1->SetSize(30.0f, 35.0f);
	mpRButtonUI = new CImage
	(
		RBUTTON_IMAGE,
		ETaskPriority::eUI, 0, ETaskPauseType::eGame,
		false, false
	);
	mpRButtonUI->SetPos(1240.0f, 650.0f);
	mpRButtonUI->SetSize(30.0f, 35.0f);
	 mpToukiUI1 = new CImage
	(
		USETOUKI_IMAGE,
		ETaskPriority::eUI, 0, ETaskPauseType::eGame,
		false, false
	);
	mpToukiUI1->SetPos(1195.0f, 530.0f);
	mpToukiUI1->SetSize(50.0f, 50.0f);
	mpToukiUI2 = new CImage
	(
		USETOUKI_IMAGE,
		ETaskPriority::eUI, 0, ETaskPauseType::eGame,
		false, false
	);
	mpToukiUI2->SetPos(1225.0f, 530.0f);
	mpToukiUI2->SetSize(50.0f, 50.0f);

	// 弱闘技UI
	mpCounterUI = new CImage
	(
		COUNTER_IMAGE,
		ETaskPriority::eUI, 0, ETaskPauseType::eGame,
		false, false
	);
	mpCounterUI->SetPos(1110.0f, 570.0f);
	mpCounterUI->SetSize(70.0f, 70.0f);
	mpCKeyUI2 = new CImage
	(
		CKEY_IMAGE,
		ETaskPriority::eUI, 0, ETaskPauseType::eGame,
		false, false
	);
	mpCKeyUI2->SetPos(1110.0f, 650.0f);
	mpCKeyUI2->SetSize(30.0f, 35.0f);
	mpLButtonUI = new CImage
	(
		LBUTTON_IMAGE,
		ETaskPriority::eUI, 0, ETaskPauseType::eGame,
		false, false
	);
	mpLButtonUI->SetPos(1150.0f, 650.0f);
	mpLButtonUI->SetSize(30.0f, 35.0f);
	mpToukiUI3 = new CImage
	(
		USETOUKI_IMAGE,
		ETaskPriority::eUI, 0, ETaskPauseType::eGame,
		false, false
	);
	mpToukiUI3->SetPos(1125.0f, 530.0f);
	mpToukiUI3->SetSize(50.0f, 50.0f);

	// HPゲージを作成
	mpHPGauge = new CHPGauge();
	mpHPGauge->SetPos(10.0f, 10.0f);

	// SPゲージを作成
	mpSPGauge = new CSPGauge();
	mpSPGauge->SetPos(10.0f, 30.0f);

	// 闘気ゲージを作成
	mpToukiGauge = new CToukiGauge();
	mpToukiGauge->SetPos(10.0f, 50.0f);

	// ドラゴンのHPゲージを作成
	mpDragonHPGauge = new CHPGauge_Dragon();
	mpDragonHPGauge->SetPos(250.0f, 600.0f);
	mpDragonHPGauge->SetShow(false);

}

// デストラクタ
CGameUI::~CGameUI()
{
	if (spInstance == this) spInstance = nullptr;

	SAFE_DELETE(mpTargetUI);
	SAFE_DELETE(mpQKeyUI);
	SAFE_DELETE(mpCKeyUI1);
	SAFE_DELETE(mpCKeyUI2);
	SAFE_DELETE(mpSPMoveUI);
	SAFE_DELETE(mpCounterUI);
	SAFE_DELETE(mpLButtonUI);
	SAFE_DELETE(mpRButtonUI);
	SAFE_DELETE(mpToukiUI1);
	SAFE_DELETE(mpToukiUI2);
	SAFE_DELETE(mpToukiUI3);

	SAFE_DELETE(mpHPGauge);
	SAFE_DELETE(mpSPGauge);
	SAFE_DELETE(mpToukiGauge);
	SAFE_DELETE(mpDragonHPGauge);
}

void CGameUI::SetHP(int hp)
{
	if (spInstance == nullptr) return;
	spInstance->mpHPGauge->SetValue(hp);
}

void CGameUI::SetMaxHP(int maxHp)
{
	if (spInstance == nullptr) return;
	spInstance->mpHPGauge->SetMaxValue(maxHp);
}

void CGameUI::SetSP(float sp)
{
	if (spInstance == nullptr) return;
	spInstance->mpSPGauge->SetValueF(sp);
}

void CGameUI::SetSPZeroFlag(bool flag)
{
	if (spInstance == nullptr) return;
	spInstance->mpSPGauge->SetSPZeroFlag(flag);
}

void CGameUI::SetMaxSP(int maxSp)
{
	if (spInstance == nullptr) return;
	spInstance->mpSPGauge->SetMaxValue(maxSp);
}

void CGameUI::SetTouki(int touki)
{
	if (spInstance == nullptr) return;
	spInstance->mpToukiGauge->SetValue(touki);
}

void CGameUI::SetMaxTouki(int maxTouki)
{
	if (spInstance == nullptr) return;
	spInstance->mpToukiGauge->SetMaxValue(maxTouki);
}

void CGameUI::SetDragonHP(int hp)
{
	if (spInstance == nullptr) return;
	spInstance->mpDragonHPGauge->SetValue(hp);
}

void CGameUI::SetDragonMaxHP(int maxHP)
{
	if (spInstance == nullptr) return;
	spInstance->mpDragonHPGauge->SetMaxValue(maxHP);
}

void CGameUI::SetShowDragonHPGauge(bool show)
{
	if (spInstance == nullptr) return;
	spInstance->mpDragonHPGauge->SetShow(show);
}

void CGameUI::SetShowUI(bool show)
{
	if (spInstance == nullptr) return;
	spInstance->SetShow(show);
}

void CGameUI::SetColorSpMove(CColor color)
{
	if (spInstance == nullptr) return;
	spInstance->mpSPMoveUI->SetColor(color);
}

void CGameUI::SetColorCounter(CColor color)
{
	if (spInstance == nullptr) return;
	spInstance->mpCounterUI->SetColor(color);
}

// 更新処理
void CGameUI::Update()
{
	mpHPGauge->Update();
	mpSPGauge->Update();
	mpToukiGauge->Update();
	mpDragonHPGauge->Update();
}

// 描画処理
void CGameUI::Render()
{
	mpHPGauge->Render();
	mpSPGauge->Render();
	mpToukiGauge->Render();
	if (mpDragonHPGauge->IsShow())
	{
		mpDragonHPGauge->Render();
	}
	// ターゲットカメラUI
	mpTargetUI->Render();
	mpQKeyUI->Render();

	// 強闘技UI
	mpSPMoveUI->Render();
	mpCKeyUI1->Render();
	mpRButtonUI->Render();
	mpToukiUI1->Render();
	mpToukiUI2->Render();

	// 弱闘技UI
	mpCounterUI->Render();
	mpCKeyUI2->Render();
	mpLButtonUI->Render();
	mpToukiUI3->Render();
}