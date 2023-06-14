#include "CMapManager.h"
#include "CTaskManager.h"

#define MODEL_WALL "res\\Wall.obj","res\\Wall.mtl"				//壁
#define MODEL_BLUEFLAG "res\\BlueFlag.obj","res\\BlueFlag.mtl"	//青フラッグ
#define MODEL_REDFLAG "res\\RedFlag.obj","res\\RedFlag.mtl"		//赤フラッグ
#define WALL_SIZE CVector(1.0f, 1.5f, 1.2f)						//壁のサイズ
#define FLAG_SIZE CVector(0.2f, 0.2f, 0.2f)						//フラッグのサイズ

CMapManager::CMapManager()
{
	//初期処理
	Init();

	// タスクマネージャーに自身を追加
	CTaskManager::Instance()->Add(this);
}

CMapManager::~CMapManager()
{
	//壁のモデルデータを破棄
	if (mpWallModel != nullptr)
	{
		delete mpWallModel;
	}
	//壁を破棄
	for (int i = 0; i < mWall.size(); i++)
	{
		delete mWall[i];
	}
	//青フラッグのモデルデータを破棄
	if (mpBlueFlagModel != nullptr)
	{
		delete mpBlueFlagModel;
	}
	//赤フラッグのモデルデータを破棄
	if (mpRedFlagModel != nullptr)
	{
		delete mpRedFlagModel;
	}
	//フラッグを破棄
	for (int i = 0; i < mFlag.size(); i++)
	{
		delete mFlag[i];
	}

	//一応リストもクリアしておく
	mWall.clear();
	mFlag.clear();

	//タスクマネージャーから自身を取り除く
	CTaskManager::Instance()->Remove(this);
}

void CMapManager::Init()
{
	//壁のモデル読み込み
	mpWallModel = new CModel();
	mpWallModel->Load(MODEL_WALL);
	//フラッグのモデル読み込み
	mpBlueFlagModel = new CModel();
	mpBlueFlagModel->Load(MODEL_BLUEFLAG);
	mpRedFlagModel = new CModel();
	mpRedFlagModel->Load(MODEL_REDFLAG);

	//壁を生成
	CreateWall(CVector(0.0f, 1.0f, 5.0f), CVector(0.0f,90.0f,0.0f), WALL_SIZE);
	CreateWall(CVector(-5.0f, 1.0f, 10.0f), CVector(0.0f,90.0f,0.0f), WALL_SIZE);
	CreateWall(CVector(7.0f, 1.0f, 15.0f), CVector(0.0f, 90.0f, 0.0f), WALL_SIZE);
	CreateWall(CVector(0.0f, 1.0f, 20.0f), CVector(0.0f, 90.0f, 0.0f), WALL_SIZE);
	CreateWall(CVector(-5.0f, 1.0f, 25.0f), CVector(0.0f, 90.0f, 0.0f), WALL_SIZE);
	CreateWall(CVector(7.0f, 1.0f, 30.0f), CVector(0.0f, 90.0f, 0.0f), WALL_SIZE);
	CreateWall(CVector(0.0f, 1.0f, 35.0f), CVector(0.0f, 90.0f, 0.0f), WALL_SIZE);
	//フラッグを生成
	//CreateFlag(CVector(0.0f, -1.0f, 10.5f), CVector(), FLAG_SIZE, 1);
	// CreateFlag(CVector(0.0f, -1.0f, 29.5f), CVector(0.0f, 180.0f, 0.0f), FLAG_SIZE, 0);
}

void CMapManager::CreateWall(CVector& pos, CVector& rot, CVector& scale)
{
	if (mpWallModel == nullptr) return;

	//壁を生成してリストに追加
	CWall* wall = new CWall(mpWallModel, pos, rot, scale);
	mWall.push_back(wall);
}

void CMapManager::CreateFlag(CVector& pos, CVector& rot, CVector& scale, int priority)
{
	if (mpBlueFlagModel == nullptr) return;
	if (mpRedFlagModel == nullptr) return;

	if (priority == 1)	//優先度が1の時青フラッグを生成
	{
		//青フラッグを生成してリストに追加
		CFlag* blueflag = new CFlag(mpBlueFlagModel, pos, rot, scale);
		mFlag.push_back(blueflag);
	}
	else
	{
		//赤フラッグを生成してリストに追加
		CFlag* redflag = new CFlag(mpRedFlagModel, pos, rot, scale);
		mFlag.push_back(redflag);
	}
}

void CMapManager::Update()
{
	for (int i = 0; i < mWall.size(); i++)
	{
		CWall* wall = mWall[i];
	}
	for (int i = 0; i < mFlag.size(); i++)
	{
		CFlag* flag = mFlag[i];
	}
}