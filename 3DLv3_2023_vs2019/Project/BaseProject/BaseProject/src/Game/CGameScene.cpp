#include "CGameScene.h"
#include "CSceneManager.h"
#include "CGameCamera.h"
#include "CPlayer.h"
#include "CDragon.h"
#include "CField.h"
#include "CInput.h"
#include "CGameMenu.h"
#include "CBGMManager.h"
#include "Global.h"
#include "CGameOverScene.h"
#include "CGameClearScene.h"
#include "CBGMManager.h"

CField* gField = nullptr;

//コンストラクタ
CGameScene::CGameScene()
	: CSceneBase(EScene::eGame)
	, mpGameMenu(nullptr)
	, mpGameOver(nullptr)
	, mpGameClear(nullptr)
{
}

//デストラクタ
CGameScene::~CGameScene()
{
}

//シーン読み込み
void CGameScene::Load()
{
	// ゲーム画面はカーソル非表示
	CInput::ShowCursor(false);
	// 背景色設定
	System::SetClearColor(0.1921569f, 0.3019608f, 0.4745098f, 1.0f);

	//ここでゲーム中に必要な
	//リソースの読み込みやクラスの生成を行う
	CResourceManager::Load<CModel>("Field", "Field\\Coliseum\\Coliseum.obj");
	CResourceManager::Load<CModel>("Cylinder", "Field\\Coliseum\\Cylinder.obj");
	CResourceManager::Load<CModelX>("Player", "Character\\Player\\player.x");
	CResourceManager::Load<CModel>("Sword", "Character\\Sword\\sword.obj");
	CResourceManager::Load<CModelX>("Dragon", "Character\\Dragon\\Dragon.x");	
	CResourceManager::Load<CTexture>("NormalSwordEffect", "Effect\\NormalAttack.png");
	//CResourceManager::Load<CTexture>("Laser", "Effect\\laser.png");
	//CResourceManager::Load<CSound>("SlashSound", "Sound\\SE\\slash.wav");
	
	// ゲームBGMを読み込み
	mpGameBGM = CResourceManager::Load<CSound>("GameBGM", "Sound\\BGM\\battle_bgm.wav");
	mpNature = CResourceManager::Load<CSound>("Nature", "Sound\\BGM\\nature.wav");

	//フィールドを生成
	gField = new CField();
	//プレイヤーを生成
	CPlayer* player = new CPlayer();
	player->Scale(10.0f, 10.0f, 10.0f);
	player->Rotate(0.0f, 180.0f, 0.0f);
	player->Position(0.0f, 50.0f, 200.0f);

	// ドラゴン(敵)を生成
	CDragon* dragon = new CDragon();
	dragon->Position(-20.0f, 50.0f, -400.0f);
	dragon->Scale(0.14f, 0.14f, 0.14f);

	//カメラセット
	CVector atPos = player->Position() + CVector(0.0f, 10.0f, 0.0f);
	CGameCamera* mainCamera = new CGameCamera
	(
		atPos + CVector(0.0f, 0.0f, 40.0f),
		atPos
	);
	//メインカメラの追従ターゲットをプレイヤーに設定
	mainCamera->SetFollowTargetTf(player);
	// メインカメラの衝突判定にフィールドのコライダーを追加
	mainCamera->AddCollider(gField->GetFieldCol());

	// ゲームメニューを作成
	mpGameMenu = new CGameMenu();

	// リザルトシーンを作成
	mpGameOver = new CGameOverScene();
	mpGameClear = new CGameClearScene();

	//CLineEffect* le = new CLineEffect(ETag::eNone);
//le->AddPoint(CVector(0.0f, 10.0f, 10.0f), 1.0f);
//le->AddPoint(CVector(10.0f, 10.0f, 10.0f), 1.0f);
//le->AddPoint(CVector(10.0f, 20.0f, 50.0f), 1.0f);
//le->AddPoint(CVector(50.0f, 10.0f, 50.0f), 1.0f);

}

//シーンの更新処理
void CGameScene::Update()
{
	if (!mpNature->IsPlaying())
	{
		// 環境音を再生
		mpNature->PlayLoop(-1, true, 0.08f, 0.0f);
	}

	CDragon* dragon = CDragon::Instance();
	if (dragon->IsFoundPlayer())
	{
		// BGM再生中でなければ、BGMを再生
		if (!mpGameBGM->IsPlaying())
		{
			mpGameBGM->PlayLoop(-1, true, 0.09f, false, 0.0f);
			mpNature->SetVolume(0.01);
		}
	}

	if (CInput::PushKey('T'))
	{
		CSceneManager::Instance()->LoadScene(EScene::eTitle);
	}

	// ゲームメニューを開いてなければ、[Ｍ]キーでメニューを開く
	if (!mpGameMenu->IsOpened())
	{
		if (CInput::PushKey('M'))
		{
			mpGameMenu->Open();
		}
	}
#ifdef _DEBUG
	// リザルトシーン
	if (!mpGameOver->IsPlayResult())
	{
		if (CInput::PushKey('O'))
		{
			mpGameOver->Start();
		}
	}
	else
	{
		if (CInput::PushKey('O'))
		{
			mpGameOver->End();
		}
	}

	if (!mpGameClear->IsPlayResult())
	{
		if (CInput::PushKey('I'))
		{
			mpGameClear->Start();
		}
	}
	else
	{
		if (CInput::PushKey('I'))
		{
			mpGameClear->End();
		}
	}
#endif
}