#include "CTitleScene.h"
#include "CSceneManager.h"
#include "CGameScene.h"
#include "CSound.h"
#include "CBGMManager.h"
#include "CTitleUI.h"
#include "CModel.h"
#include "CTitleCamera.h"
#include "CTitleField.h"
#include "CTitleModel.h"

//コンストラクタ
CTitleScene::CTitleScene()
	: CSceneBase(EScene::eTitle)
{
}

//デストラクタ
CTitleScene::~CTitleScene()
{
}

//シーン読み込み
void CTitleScene::Load()
{
	// タイトル画面はカーソル表示
	CInput::ShowCursor(true);
	// 背景色設定
	//System::SetClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	// 背景色設定
	System::SetClearColor(0.1921569f, 0.3019608f, 0.4745098f, 1.0f);

	//ここでタイトル画面に必要な
	//リソースの読み込みやクラスの生成を行う
	CResourceManager::Load<CModel>("TitleField", "Field\\WaitingRoom\\WaitingRoom.obj");
	CResourceManager::Load<CModelX>("TitleModel", "Character\\Player\\player.x");
	CResourceManager::Load<CModel>("Sword", "Character\\Sword\\sword.obj");
	//CResourceManager::Load<CSound>("ButtonSE", "Sound\\SE\\button_se.wav");
	
	// タイトルBGMを再生
	mpTitleBGM = CResourceManager::Load<CSound>("TitleBGM","Sound\\BGM\\title_bgm.wav");

	// タイトル用のステージを生成
	CTitleField* titleField = new CTitleField();
	titleField->Scale(100.0f, 100.0f, 100.0f);
	titleField->Position(0.0f, -30.0f, 0.0f);

	// タイトル用のプレイヤーモデルの生成
	CTitleModel* titleModel = new CTitleModel();
	titleModel->Scale(80.0f, 80.0f, 80.0f);
	titleModel->Rotate(0.0f, 180.0f, 0.0f);
	titleModel->Position(0.0f, 10.0f, 0.0f);

	CVector tPos = titleModel->Position();
	CTitleCamera* titleCamera = new CTitleCamera
	(
		tPos + CVector(25.0f, 108.0f, -70.0f),
		tPos + CVector(30.0f, 108.0f, 0.0f)
	);

	//メインカメラの追従ターゲットをプレイヤーに設定
	//titleCamera->SetFollowTargetTf(player);

	mpTitleUI = new CTitleUI();
	AddTask(mpTitleUI);

	mpButton = CResourceManager::Load<CSound>("ButtonSE", "Sound\\SE\\button_se.wav");
}

//シーンの更新処理
void CTitleScene::Update()
{
	if (!mpTitleBGM->IsPlaying())
	{
		mpTitleBGM->PlayLoop(-1, true, 0.2f, false, 0.0f);
	}
	// タイトル画面が終了
	if (mpTitleUI->IsEnd())
	{
		// ゲーム開始ならば、ゲームシーンを読み込む
		if (mpTitleUI->IsStartGame())
		{
			CSceneManager::Instance()->LoadScene(EScene::eGame);
			mpButton->Play(0.1f, false, 0.0f);
			mpTitleBGM->Stop();
		}
		// ゲーム終了ならば、アプリを閉じる
		else if (mpTitleUI->IsExitGame())
		{
			System::ExitGame();
			mpButton->Play(0.1f, false, 0.0f);
			mpTitleBGM->Stop();
		}
	}
}
