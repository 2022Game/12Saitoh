#include "CGameScene.h"
#include "Global.h"
#include "CPlayer.h"
#include "CSceneManager.h"

//敵の拡縮
#define ENEMY_SIZE CVector(0.045, 0.045, 0.045)

CMapManager* gMap = nullptr;
CPlayer* gPlayer = nullptr;
CEnemy* gEnemy = nullptr;
int CGameScene::mCameraFlag = 0;

//コンストラクタ
CGameScene::CGameScene()
	: CSceneBase(EScene::eGame)
	, mpUi(nullptr)
{
	delete gMap;
	gMap = nullptr;
}

//デストラクタ
CGameScene::~CGameScene()
{
	delete gPlayer;
	gPlayer = nullptr;
	delete mpUi;
	mpUi = nullptr;
}

//シーン読み込み
void CGameScene::Load()
{
	gMap = new CMapManager();
	gMap->Init();

	//背景生成
	AddTask(new CField());

	//プレイヤー生成
	gPlayer = new CPlayer();
	gPlayer->Scale(CVector(0.045f, 0.045f, 0.045f));
	gPlayer->Position(CVector(0.0f, 0.0f, -3.0f));
	gPlayer->Rotation(CVector(0.0f, 0.0f, 0.0f));
	AddTask(gPlayer);

	gEnemy = new CEnemy(CVector(-10.0f, 0.0f, 80.0f), CVector(), ENEMY_SIZE);
	AddTask(gEnemy);
	gEnemy = new CEnemy(CVector(-5.0f, 0.0f, 30.0f), CVector(0.0f, 180.0f, 0.0f), ENEMY_SIZE);
	AddTask(gEnemy);
	gEnemy = new CEnemy(CVector(40.0f, 0.0f, 40.0f), CVector(0.0f, -90.0f, 0.0f), ENEMY_SIZE);
	AddTask(gEnemy);
	gEnemy = new CEnemy(CVector(-30.0f, 0.0f, 60.0f), CVector(0.0f, 90.0f, 0.0f), ENEMY_SIZE);
	AddTask(gEnemy);\
	mpUi = new CUi();
}

//シーンの更新処理
void CGameScene::Update()
{
	//デバッグ用(カメラの切り替え)
	if (mInput.PullKey('C'))
	{
		mCameraFlag++;
		if (mCameraFlag >= 3)
		{
			mCameraFlag = 0;
		}
	}
	//プレイヤーが存在しなければ処理しない
	CPlayer* player = CPlayer::Instance();
	if (player == nullptr) return;
	//敵が存在しなければ処理しない
	CEnemy* enemy = CEnemy::Instance();
	if (enemy == nullptr) return;

	//カメラのパラメータを作成する
	CVector e, c, u; //視点、注視点、上方向
	if (mCameraFlag == 0)
	{
		//視点を求める
		e = player->Position() + CVector(0.0f, 3.0f, 0.1f) * player->MatrixRotate();
		//注視点を求める
		c = player->Position() + CVector(0.0f, 3.0f, 1.0f) * player->MatrixRotate();
		//上方向を求める
		u = CVector(0.0f, 0.1f, 0.0f) * player->MatrixRotate();
		//カメラの設定
		gluLookAt(e.X(), e.Y(), e.Z(), c.X(), c.Y(), c.Z(), u.X(), u.Y(), u.Z());
	}
	else if (mCameraFlag == 1)
	{
		//視点を求める
		e = player->Position() + CVector(-0.9f, 3.0f, -12.0f) * player->MatrixRotate();
		//注視点を求める
		c = player->Position();
		//上方向を求める
		u = CVector(0.0f, 1.0f, 0.0f) * player->MatrixRotate();
		//カメラの設定
		gluLookAt(e.X(), e.Y(), e.Z(), c.X(), c.Y(), c.Z(), u.X(), u.Y(), u.Z());
	}
	else
	{
		//視点を求める
		e = player->Position() + CVector(0.5f, 3.0f, 12.0f) * player->MatrixRotate();
		//注視点を求める
		c = player->Position();
		//上方向を求める
		u = CVector(0.0f, 1.0f, 0.0f) * player->MatrixRotate();
		//カメラの設定
		gluLookAt(e.X(), e.Y(), e.Z(), c.X(), c.Y(), c.Z(), u.X(), u.Y(), u.Z());
	}

	//プレイヤーが死亡したらゲームオーバー画面に移行する
	if (player->IsDeath())
	{
		CSceneManager::Instance()->LoadScene(EScene::eOver);
		enemy->ResetNum();
		if (gMap != nullptr)
		{
			delete gMap;
			gMap = nullptr;
		}
	}
	//敵の数が0になったらゲームクリア画面に移行する
	else if (enemy->Num() <= 0)
	{
		CSceneManager::Instance()->LoadScene(EScene::eClear);
		enemy->ResetNum();
		if (gMap != nullptr)
		{
			delete gMap;
			gMap = nullptr;
		}
	}
}