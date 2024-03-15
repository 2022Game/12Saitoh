#include "CGameScene.h"
#include "CSceneManager.h"
#include "CField.h"
#include "CPlayer.h"
#include "CEnemy.h"
#include "CDragon.h"
#include "CGameCamera.h"
#include "CInput.h"
#include "CGameMenu.h"
#include "CGameMenu.h"
#include "CBGMManager.h"

//コンストラクタ
CGameScene::CGameScene()
	: CSceneBase(EScene::eGame)
	, mpGameMenu(nullptr)
{
}

//デストラクタ
CGameScene::~CGameScene()
{
}

//シーン読み込み
void CGameScene::Load()
{
	// 背景色設定
	System::SetClearColor(0.1921569f, 0.3019608f, 0.4745098f, 1.0f);
	//ここでゲーム中に必要な
	//リソースの読み込みやクラスの生成を行う

	CResourceManager::Load<CModel>("Field", "Field\\field.obj");
	CResourceManager::Load<CModel>("FieldCamCol", "Field\\fieldCamCol.obj");
	CResourceManager::Load<CModel>("FieldCube", "Field\\Object\\cube.obj");
	CResourceManager::Load<CModel>("FieldCylinder", "Field\\Object\\cylinder.obj");
	CResourceManager::Load<CModelX>("Player", "Character\\Player\\player.x");
	CResourceManager::Load<CModel>("Sword", "Character\\Sword\\sword.obj");
	//CResourceManager::Load<CModelX>("Enemy", "Character\\Enemy\\Dragon.x");
	CResourceManager::Load<CModelX>("Dragon", "Character\\Dragon\\Dragon.x");
	CResourceManager::Load<CTexture>("Laser", "Effect\\laser.png");
	CResourceManager::Load<CSound>("SlashSound", "Sound\\SE\\slash.wav");

	// ゲームBGMを読み込み
	CBGMManager::Instance()->Play(EBGMType::eGame);

	//フィールドを生成
	CField * field = new CField();

	//プレイヤーを生成
	CPlayer* player = new CPlayer();
	player->Scale(10.0f, 10.0f, 10.0f);
	player->Rotate(0.0f, 180.0f, 0.0f);

	// ドラゴン(敵)を生成
	CDragon* dragon = new CDragon();
	dragon->Position(0.0f, 0.0f, -250.0f);
	dragon->Scale(0.15f, 0.15f, 0.15f);
	// 敵を生成
	//CEnemy* enemy = new CEnemy();
	//enemy->Position(-50.0f, 0.0f, -50.0f);
	//enemy->Scale(20.0f, 20.0f, 20.0f);

	////カメラセット
	//CGameCamera* mainCamera = new CGameCamera
	//(
	//	CVector(0.0f, 30.0f, 40.0f),
	//	player->Position()
	//);
	
	// CGameCameraのテスト
	CVector atPos = player->Position() + CVector(0.0f, 10.0f, 0.0f);
	CGameCamera* mainCamera = new CGameCamera
	(
		atPos + CVector(0.0f, 0.0f, 40.0f),
		atPos
	);

	//メインカメラの追従ターゲットをプレイヤーに設定
	mainCamera->SetFollowTargetTf(player);
	// メインカメラの衝突判定にフィールドのコライダーを追加
	mainCamera->AddCollider(field->GetFieldCol());
	// ゲームメニューを作成
	mpGameMenu = new CGameMenu();

	//CLineEffect* le = new CLineEffect(ETag::eNone);
//le->AddPoint(CVector(0.0f, 10.0f, 10.0f), 1.0f);
//le->AddPoint(CVector(10.0f, 10.0f, 10.0f), 1.0f);
//le->AddPoint(CVector(10.0f, 20.0f, 50.0f), 1.0f);
//le->AddPoint(CVector(50.0f, 10.0f, 50.0f), 1.0f);

}

//シーンの更新処理
void CGameScene::Update()
{
	// BGM再生中でなければ、BGMを再生
//if (!mpGameBGM->IsPlaying())
//{
//	mpGameBGM->PlayLoop(-1, 1.0f, false, 1.0f);
//}

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
}