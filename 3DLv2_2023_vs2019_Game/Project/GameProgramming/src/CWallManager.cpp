#include "CWallManager.h"
#include "CTaskManager.h"

//土台Coin
#define MODEL_WALL "res\\Wall.obj","res\\Wall.mtl"

CWallManager::CWallManager()
{
	//初期処理
	Init();

	// タスクマネージャーに自身を追加
	CTaskManager::Instance()->Add(this);
}

CWallManager::~CWallManager()
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
	//一応リストもクリアしておく
	mWall.clear();

	//タスクマネージャーから自身を取り除く
	CTaskManager::Instance()->Remove(this);
}

void CWallManager::Init()
{
	//壁のモデル読み込み
	mpWallModel = new CModel();
	mpWallModel->Load(MODEL_WALL);

	//壁を生成
	CreateWall(CVector(0.0f, 0.1f, 0.0f), CVector(0.0f,90.0f,0.0f), CVector(0.8f, 0.8f, 0.8f));
	CreateWall(CVector(0.0f, 0.1f, 2.0f), CVector(), CVector(0.8f, 0.8f, 0.8f));
	CreateWall(CVector(0.0f, 0.1f, 4.0f), CVector(), CVector(0.8f, 0.8f, 0.8f));
	CreateWall(CVector(0.0f, 0.1f, 6.0f), CVector(), CVector(0.8f, 0.8f, 0.8f));
	CreateWall(CVector(0.0f, 0.1f, 8.0f), CVector(), CVector(0.8f, 0.8f, 0.8f));
	CreateWall(CVector(0.0f, 0.1f, 10.0f), CVector(), CVector(0.8f, 0.8f, 0.8f));
}

void CWallManager::CreateWall(CVector& pos, CVector& rot, CVector& scale)
{
	if (mpWallModel == nullptr) return;

	//壁を生成してリストに追加
	CWall* wall = new CWall(mpWallModel, pos, rot, scale);
	mWall.push_back(wall);
}

void CWallManager::Update()
{
	for (int i = 0; i < mWall.size(); i++)
	{
		CWall* wall = mWall[i];
	}
}