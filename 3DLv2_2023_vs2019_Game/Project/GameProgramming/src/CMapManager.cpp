#include "CMapManager.h"
#include "CTaskManager.h"
#include "NavManager.h"

#define MODEL_WALL "res\\Wall.obj","res\\Wall.mtl"				//壁
#define MODEL_BLUEFLAG "res\\BlueFlag.obj","res\\BlueFlag.mtl"	//青フラッグ
#define MODEL_REDFLAG "res\\RedFlag.obj","res\\RedFlag.mtl"		//赤フラッグ
#define WALL_SIZE CVector(2.0f, 3.0f, 2.4f)						//壁のサイズ
#define FLAG_SIZE CVector(0.2f, 0.2f, 0.2f)						//フラッグのサイズ

CMapManager* CMapManager::spInstance = nullptr;

CMapManager::CMapManager()
	: mpWallModel(nullptr)
	, mpRedFlagModel(nullptr)
	, mpBlueFlagModel(nullptr)
{

}

CMapManager::~CMapManager()
{
	//壁のモデルデータを破棄
	if (mpWallModel != nullptr)
	{
		delete mpWallModel;
		mpWallModel = nullptr;
	}
	//壁を破棄
	for (int i = 0; i < mWall.size(); i++)
	{
		if (mWall[i] != nullptr)
		{
			delete mWall[i];
			mWall[i] = nullptr;
		}
	}
	//青フラッグのモデルデータを破棄
	if (mpBlueFlagModel != nullptr)
	{
		delete mpBlueFlagModel;
		mpBlueFlagModel = nullptr;
	}
	//赤フラッグのモデルデータを破棄
	if (mpRedFlagModel != nullptr)
	{
		delete mpRedFlagModel;
		mpRedFlagModel = nullptr;
	}
	//フラッグを破棄
	for (int i = 0; i < mFlag.size(); i++)
	{
		if (mFlag[i] != nullptr)
		{
			delete mFlag[i];
			mFlag[i] = nullptr;
		}
	}
	//三角コライダを破棄
	for (int i = 0; i < mColliderTriangle.size(); i++)
	{
		if (mColliderTriangle[i] != nullptr)
		{
			delete mColliderTriangle[i];
			mColliderTriangle[i] = nullptr;
		}
	}

	//一応リストもクリアしておく
	mWall.clear();
	mFlag.clear();
	mColliderTriangle.clear();
}

CMapManager* CMapManager::Instance()
{
	if (spInstance == nullptr)
	{
		spInstance = new CMapManager();
	}
	return spInstance;
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
	CreateWall(CVector(0.0f, -1.0f, 10.0f), CVector(0.0f,90.0f,0.0f), WALL_SIZE);
	CreateWall(CVector(-20.0f, -1.0f, 30.0f), CVector(0.0f,90.0f,0.0f), WALL_SIZE);
	CreateWall(CVector(24.0f, -1.0f, 40.0f), CVector(0.0f, 90.0f, 0.0f), WALL_SIZE);
	CreateWall(CVector(0.0f, -1.0f, 50.0f), CVector(0.0f, 90.0f, 0.0f), WALL_SIZE);
	CreateWall(CVector(-20.0f, -1.0f, 60.0f), CVector(0.0f, 90.0f, 0.0f), WALL_SIZE);
	CreateWall(CVector(24.0f, -1.0f, 70.0f), CVector(0.0f, 90.0f, 0.0f), WALL_SIZE);
	CreateWall(CVector(0.0f, -1.0f, 80.0f), CVector(0.0f, 90.0f, 0.0f), WALL_SIZE);
	//フラッグを生成
	//CreateFlag(CVector(0.0f, -1.0f, 10.5f), CVector(), FLAG_SIZE, 1);
	//CreateFlag(CVector(0.0f, -1.0f, 29.5f), CVector(0.0f, 180.0f, 0.0f), FLAG_SIZE, 0);

	//壁の周りの4隅に経路探索用のノードを生成
	for (int i = 0; i < mWall.size(); i++)
	{
		CVector pos = mWall[i]->Position();
		new NavNode(pos + CVector(5.0f, 0.5f, -2.0f) * 2);
		new NavNode(pos + CVector(5.0f, 0.5f, 2.0f) * 2);
		new NavNode(pos + CVector(-5.0f, 0.5f, 2.0f) * 2);
		new NavNode(pos + CVector(-5.0f, 0.5f, -2.0f) * 2);
	}

	//フィールドの設定
	//1枚目(真ん中)
	CreateColliderTriangle(nullptr, nullptr
		, CVector(40.0f, 20.0f, -10.0f)
		, CVector(-40.0f, -1.0f, -10.0f)
		, CVector(40.0f, -1.0f, -10.0f));
	CreateColliderTriangle(nullptr, nullptr
		, CVector(-40.0f, 20.0f, -10.0f)
		, CVector(-40.0f, -1.0f, -10.0f)
		, CVector(40.0f, 20.0f, -10.0f));
	//2枚目(側面)
	CreateColliderTriangle(nullptr, nullptr
		, CVector(40.0f, 20.0f, 100.0f)
		, CVector(40.0f, -1.0f, -10.0f)
		, CVector(40.0f, -1.0f, 100.0f));
	CreateColliderTriangle(nullptr, nullptr
		, CVector(40.0f, -1.0f, -10.0f)
		, CVector(40.0f, 20.0f, 100.0f)
		, CVector(40.0f, 20.0f, -10.0f));
	//3枚目(側面)
	CreateColliderTriangle(nullptr, nullptr
		, CVector(-40.0f, 20.0f, -10.0f)
		, CVector(-40.0f, -1.0f, 100.0f)
		, CVector(-40.0f, -1.0f, -10.0f));
	CreateColliderTriangle(nullptr, nullptr
		, CVector(-40.0f, -1.0f, 100.0f)
		, CVector(-40.0f, 20.0f, -10.0f)
		, CVector(-40.0f, 20.0f, 100.0f));
	//4枚目(真ん中)
	CreateColliderTriangle(nullptr, nullptr
		, CVector(-40.0f, 20.0f, 100.0f)
		, CVector(40.0f, -1.0f, 100.0f)
		, CVector(-40.0f, -1.0f, 100.0f));
	CreateColliderTriangle(nullptr, nullptr
		, CVector(40.0f, -1.0f, 100.0f)
		, CVector(-40.0f, 20.0f, 100.0f)
		, CVector(40.0f, 20.0f, 100.0f));
}

//フィールドに配置されているオブジェクトとのレイ判定
bool CMapManager::CollisionRay(const CVector& start, const CVector& end, float* outDistance)
{
	float nearDistance = -1.0f;
	//壁とのレイ判定
	for (CWall* wall : mWall)
	{
		if (wall->CollisionRay(start, end, outDistance))
		{
			if (nearDistance < 0.0f || *outDistance < nearDistance)
			{
				nearDistance = *outDistance;
			}
		}
	}
	if (nearDistance >= 0.0f)
	{
		*outDistance = nearDistance;
		return true;
	}
	return false;
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

void CMapManager::CreateColliderTriangle(CCharacter3* parent, CMatrix* matrix,
	const CVector& v0, const CVector& v1, const CVector& v2)
{
	CColliderTriangle* collider = new CColliderTriangle();
	collider->Set(parent, matrix, v0, v1, v2);
	mColliderTriangle.push_back(collider);
}