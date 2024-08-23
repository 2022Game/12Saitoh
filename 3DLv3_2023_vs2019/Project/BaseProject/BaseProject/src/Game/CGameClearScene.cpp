#include "CGameClearScene.h"
#include "CTaskManager.h"
#include "CSceneManager.h"
#include "CSceneBase.h"
#include "CSound.h"

// コンストラクタ
CGameClearScene::CGameClearScene()
{
	// ゲームクリア用UIの生成
	mpGameClearUI = new CGameClearUI();

	SetEnable(false);
	SetShow(false);
	mpButton = CResourceManager::Load<CSound>("ButtonSE", "Sound\\SE\\button_se.wav");

	mpButton->SetSimultaneousPlayCount(1);
};

// デストラクタ
CGameClearScene::~CGameClearScene()
{
	SAFE_DELETE(mpGameClearUI);
}

// 更新処理
void CGameClearScene::Update()
{
	if (!CSceneManager::Instance()->IsReady()) return;
	// ベースクラスの更新
	CResultScene::Update();
	// UIの更新
	mpGameClearUI->Update();

	// ゲームクリア画面が終了
	if (mpGameClearUI->IsEnd())
	{
		// リトライならば、ゲームシーンを読み込む
		if (mpGameClearUI->IsReTry())
		{
			CSceneManager::Instance()->LoadScene(EScene::eGame);
			mpButton->Play(0.1f, false, 0.0f);
		}
		// タイトルへ戻るならば、タイトルシーンを読み込む
		else if (mpGameClearUI->IsTitle())
		{
			CSceneManager::Instance()->LoadScene(EScene::eTitle);
			mpButton->Play(0.1f, false, 0.0f);
		}
	}
}

// 描画処理
void CGameClearScene::Render()
{
	// ベースクラスの描画処理
	CResultScene::Render();
	// UIの描画処理
	mpGameClearUI->Render();
}

// ゲームクリア開始
void CGameClearScene::Start()
{
	CResultScene::Start();
	mpGameClearUI->Start();
}

// ゲームクリア終了
void CGameClearScene::End()
{
	CResultScene::End();
	mpGameClearUI->End();
}