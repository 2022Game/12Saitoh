#include "CGameOverUI.h"
#include "CText.h"
#include "CTaskManager.h"
#include "CLightingButton.h"

// コンストラクタ
CGameOverUI::CGameOverUI()
{
	// リザルトロゴを生成
	mpResultLogo = new CText
	(
		mpResultFont, 128,
		CVector2(0.0f, 32.0f),
		CVector2(WINDOW_WIDTH, WINDOW_HEIGHT),
		CColor(1.01f, 0.1f, 0.1f),
		ETaskPriority::eUI,
		0,
		ETaskPauseType::eDefault,
		false,
		false
	);
	mpResultLogo->SetText("GameOver");
	mpResultLogo->SetEnableOutline(true);
	mpResultLogo->SetOutlineColor(CColor(0.9f, 0.9f, 0.9f));

	// [タイトルへ戻る]ボタンを生成
	CLightingButton* btn1 = new CLightingButton
	(
		CVector2(400.0f, 320.0f),
		CVector2(181.0f * 1.5f, 47.0f * 1.5f),
		ETaskPriority::eUI, 0, ETaskPauseType::eSystem,
		false, false
	);
	// ボタンの画像を読み込み
	btn1->LoadButtonImage("UI/result_title.png", "UI/result_title.png");
	// ボタンクリック時に呼び出されるコールバック関数を設定
	btn1->SetOnClickFunc(std::bind(&CResultUIBase::OnClickTitle, this));
	btn1->SetEnable(false);
	btn1->SetScale(1.0f);
	// ボタンリストに追加
	mButton.push_back(btn1);

	SetEnable(false);
	SetShow(false);
}

// デストラクタ
CGameOverUI::~CGameOverUI()
{
	// ポーズを解除
	CTaskManager::Instance()->UnPause(PAUSE_MENU_OPEN);
}

// 更新処理
void CGameOverUI::Update()
{
	mpResultLogo->Update();
	for (CButton* btn : mButton)
	{
		btn->Update();
	}
}

// 描画処理
void CGameOverUI::Render()
{
	// リザルトロゴ描画
	mpResultLogo->Render();

	for (CButton* btn : mButton)
	{
		btn->Render();
	}
}

// UI処理開始
void CGameOverUI::Start()
{
	for (CButton* btn : mButton)
	{
		btn->SetEnable(true);
	}

	SetEnable(true);
	SetShow(true);
	CTaskManager::Instance()->Pause(PAUSE_MENU_OPEN);
}

// UI処理終了
void CGameOverUI::End()
{
	for (CButton* btn : mButton)
	{
		btn->SetEnable(false);
	}
	SetEnable(false);
	SetShow(false);
	// ポーズを解除
	CTaskManager::Instance()->UnPause(PAUSE_MENU_OPEN);
}
