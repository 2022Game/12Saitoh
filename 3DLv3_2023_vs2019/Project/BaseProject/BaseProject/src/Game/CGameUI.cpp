#include "CGameUI.h"
#include "CImage.h"

// コンストラクタ
CGameUI::CGameUI()
{
	// ターゲットカメラUI
	mpTargetUI = new CImage(TARGET_IMAGE);
	mpTargetUI->SetPos(1200.0f, 20.0f);
	mpTargetUI->SetSize(50.0f, 50.0f);
	mpQKeyUI = new CImage(QKEY_IMAGE);
	mpQKeyUI->SetPos(1210.0f, 73.0f);
	mpQKeyUI->SetSize(30.0f, 35.0f);

	// 強闘技UI
	mpSPMoveUI = new CImage(SPMOVE_IMAGE);
	mpSPMoveUI->SetPos(1200.0f, 570.0f);
	mpSPMoveUI->SetSize(70.0f, 70.0f);
	mpCKeyUI1 = new CImage(CKEY_IMAGE);
	mpCKeyUI1->SetPos(1200.0f, 650.0f);
	mpCKeyUI1->SetSize(30.0f, 35.0f);
	mpRButtonUI = new CImage(RBUTTON_IMAGE);
	mpRButtonUI->SetPos(1240.0f, 650.0f);
	mpRButtonUI->SetSize(30.0f, 35.0f);

	// 弱闘技UI
	mpCounterUI = new CImage(COUNTER_IMAGE);
	mpCounterUI->SetPos(1110.0f, 570.0f);
	mpCounterUI->SetSize(70.0f, 70.0f);
	mpCKeyUI2 = new CImage(CKEY_IMAGE);
	mpCKeyUI2->SetPos(1110.0f, 650.0f);
	mpCKeyUI2->SetSize(30.0f, 35.0f);
	mpLButtonUI = new CImage(LBUTTON_IMAGE);
	mpLButtonUI->SetPos(1150.0f, 650.0f);
	mpLButtonUI->SetSize(30.0f, 35.0f);

}

// デストラクタ
CGameUI::~CGameUI()
{
	mpTargetUI->Kill();
	mpQKeyUI->Kill();
	mpCKeyUI1->Kill();
	mpCKeyUI2->Kill();
	mpSPMoveUI->Kill();
	mpCounterUI->Kill();
	mpLButtonUI->Kill();
	mpRButtonUI->Kill();
}

// 更新処理
void CGameUI::Update()
{
}