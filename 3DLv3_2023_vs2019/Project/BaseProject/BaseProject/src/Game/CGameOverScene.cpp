#include "CGameOverScene.h"
#include "CTaskManager.h"
#include "CSceneManager.h"
#include "CSceneBase.h"

// コンストラクタ
CGameOverScene::CGameOverScene()
{
	// ゲームオーバー用UIの生成
	mpGameOverUI = new CGameOverUI();

	SetEnable(false);
	SetShow(false);
};

// デストラクタ
CGameOverScene::~CGameOverScene()
{
	SAFE_DELETE(mpGameOverUI);
}

// 更新処理
void CGameOverScene::Update()
{
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
		}
		// タイトルへ戻るならば、タイトルシーンを読み込む
		else if (mpGameOverUI->IsTitle())
		{
			CSceneManager::Instance()->LoadScene(EScene::eTitle);
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