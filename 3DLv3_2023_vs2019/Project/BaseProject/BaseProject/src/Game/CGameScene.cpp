#include "CGameScene.h"
#include "CSceneManager.h"
#include "CField.h"
#include "CPlayer.h"
#include "CCamera.h"
#include "CSword.h"

//コンストラクタ
CGameScene::CGameScene()
	: CSceneBase(EScene::eGame)
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

	//フィールドを生成
	new CField();

	//プレイヤーを生成
	CPlayer* player = new CPlayer();
	player->Scale(10.0f, 10.0f, 10.0f);
	player->Rotate(0.0f, 180.0f, 0.0f);

	//剣を生成
	CSword* sword = new CSword();
	sword->Rotate(CVector(0.0f, 0.0f, 0.0f));

	//カメラセット
	CCamera* mainCamera = new CCamera
	(
		CVector(0.0f, 20.0f, 50.0f),
		player->Position()
	);
	//メインカメラの追従ターゲットをプレイヤーに設定
	mainCamera->SetFollowTargetTf(player);
}

//シーンの更新処理
void CGameScene::Update()
{
}
