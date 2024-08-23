#include "CGameOverScene.h"
#include "CTaskManager.h"
#include "CSceneManager.h"
#include "CSceneBase.h"
#include "CSound.h"

// コンストラクタ
CGameOverScene::CGameOverScene()
{
	// ゲームオーバー用UIの生成
	mpGameOverUI = new CGameOverUI();

	SetEnable(false);
	SetShow(false);
	mpButton = CResourceManager::Load<CSound>("ButtonSE", "Sound\\SE\\button_se.wav");
};

// デストラクタ
CGameOverScene::~CGameOverScene()
{
	SAFE_DELETE(mpGameOverUI);
}

// 更新処理
void CGameOverScene::Update()
{
	if (!CSceneManager::Instance()->IsReady()) return;
	// ベースクラスの更新
	CResultScene::Update();
	// UIの更新
	mpGameOverUI->Update();

	// ゲームオーバー画面が終了
	if (mpGameOverUI->IsEnd())
	{
		// リトライならば、ゲームシーンを読み込む
		if (mpGameOverUI->IsReTry())
		{
			CSceneManager::Instance()->LoadScene(EScene::eGame);
			mpButton->Play(0.1f, false, 0.0f);
		}
		// タイトルへ戻るならば、タイトルシーンを読み込む
		else if (mpGameOverUI->IsTitle())
		{
			CSceneManager::Instance()->LoadScene(EScene::eTitle);
			mpButton->Play(0.1f, false, 0.0f);
		}
	}
}

// 描画処理
void CGameOverScene::Render()
{
	// ベースクラスの描画処理
	CResultScene::Render();
	// UIの描画処理
	mpGameOverUI->Render();
}

// ゲームオーバー開始
void CGameOverScene::Start()
{
	CResultScene::Start();
	mpGameOverUI->Start();
}

void CGameOverScene::End()
{
	CResultScene::End();
	mpGameOverUI->End();
}