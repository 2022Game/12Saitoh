#include "CMapManager.h"
#include "CTaskManager.h"
#include "NavManager.h"

#define MODEL_WALL "res\\Wall.obj","res\\Wall.mtl"				//��
#define MODEL_BLUEFLAG "res\\BlueFlag.obj","res\\BlueFlag.mtl"	//�t���b�O
#define MODEL_REDFLAG "res\\RedFlag.obj","res\\RedFlag.mtl"		//�ԃt���b�O
#define WALL_SIZE CVector(2.0f, 3.0f, 2.4f)						//�ǂ̃T�C�Y
#define FLAG_SIZE CVector(0.2f, 0.2f, 0.2f)						//�t���b�O�̃T�C�Y

CMapManager* CMapManager::spInstance = nullptr;

CMapManager::CMapManager()
	: mpWallModel(nullptr)
	, mpRedFlagModel(nullptr)
	, mpBlueFlagModel(nullptr)
{

}

CMapManager::~CMapManager()
{
	//�ǂ̃��f���f�[�^��j��
	if (mpWallModel != nullptr)
	{
		delete mpWallModel;
		mpWallModel = nullptr;
	}
	//�ǂ�j��
	for (int i = 0; i < mWall.size(); i++)
	{
		if (mWall[i] != nullptr)
		{
			delete mWall[i];
			mWall[i] = nullptr;
		}
	}
	//�t���b�O�̃��f���f�[�^��j��
	if (mpBlueFlagModel != nullptr)
	{
		delete mpBlueFlagModel;
		mpBlueFlagModel = nullptr;
	}
	//�ԃt���b�O�̃��f���f�[�^��j��
	if (mpRedFlagModel != nullptr)
	{
		delete mpRedFlagModel;
		mpRedFlagModel = nullptr;
	}
	//�t���b�O��j��
	for (int i = 0; i < mFlag.size(); i++)
	{
		if (mFlag[i] != nullptr)
		{
			delete mFlag[i];
			mFlag[i] = nullptr;
		}
	}
	//�O�p�R���C�_��j��
	for (int i = 0; i < mColliderTriangle.size(); i++)
	{
		if (mColliderTriangle[i] != nullptr)
		{
			delete mColliderTriangle[i];
			mColliderTriangle[i] = nullptr;
		}
	}

	//�ꉞ���X�g���N���A���Ă���
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
	//�ǂ̃��f���ǂݍ���
	mpWallModel = new CModel();
	mpWallModel->Load(MODEL_WALL);
	//�t���b�O�̃��f���ǂݍ���
	mpBlueFlagModel = new CModel();
	mpBlueFlagModel->Load(MODEL_BLUEFLAG);
	mpRedFlagModel = new CModel();
	mpRedFlagModel->Load(MODEL_REDFLAG);

	//�ǂ𐶐�
	CreateWall(CVector(0.0f, -1.0f, 10.0f), CVector(0.0f,90.0f,0.0f), WALL_SIZE);
	CreateWall(CVector(-20.0f, -1.0f, 30.0f), CVector(0.0f,90.0f,0.0f), WALL_SIZE);
	CreateWall(CVector(24.0f, -1.0f, 40.0f), CVector(0.0f, 90.0f, 0.0f), WALL_SIZE);
	CreateWall(CVector(0.0f, -1.0f, 50.0f), CVector(0.0f, 90.0f, 0.0f), WALL_SIZE);
	CreateWall(CVector(-20.0f, -1.0f, 60.0f), CVector(0.0f, 90.0f, 0.0f), WALL_SIZE);
	CreateWall(CVector(24.0f, -1.0f, 70.0f), CVector(0.0f, 90.0f, 0.0f), WALL_SIZE);
	CreateWall(CVector(0.0f, -1.0f, 80.0f), CVector(0.0f, 90.0f, 0.0f), WALL_SIZE);
	//�t���b�O�𐶐�
	//CreateFlag(CVector(0.0f, -1.0f, 10.5f), CVector(), FLAG_SIZE, 1);
	//CreateFlag(CVector(0.0f, -1.0f, 29.5f), CVector(0.0f, 180.0f, 0.0f), FLAG_SIZE, 0);

	//�ǂ̎����4���Ɍo�H�T���p�̃m�[�h�𐶐�
	for (int i = 0; i < mWall.size(); i++)
	{
		CVector pos = mWall[i]->Position();
		new NavNode(pos + CVector(5.0f, 0.5f, -2.0f) * 2);
		new NavNode(pos + CVector(5.0f, 0.5f, 2.0f) * 2);
		new NavNode(pos + CVector(-5.0f, 0.5f, 2.0f) * 2);
		new NavNode(pos + CVector(-5.0f, 0.5f, -2.0f) * 2);
	}

	//�t�B�[���h�̐ݒ�
	//1����(�^��)
	CreateColliderTriangle(nullptr, nullptr
		, CVector(40.0f, 20.0f, -10.0f)
		, CVector(-40.0f, -1.0f, -10.0f)
		, CVector(40.0f, -1.0f, -10.0f));
	CreateColliderTriangle(nullptr, nullptr
		, CVector(-40.0f, 20.0f, -10.0f)
		, CVector(-40.0f, -1.0f, -10.0f)
		, CVector(40.0f, 20.0f, -10.0f));
	//2����(����)
	CreateColliderTriangle(nullptr, nullptr
		, CVector(40.0f, 20.0f, 100.0f)
		, CVector(40.0f, -1.0f, -10.0f)
		, CVector(40.0f, -1.0f, 100.0f));
	CreateColliderTriangle(nullptr, nullptr
		, CVector(40.0f, -1.0f, -10.0f)
		, CVector(40.0f, 20.0f, 100.0f)
		, CVector(40.0f, 20.0f, -10.0f));
	//3����(����)
	CreateColliderTriangle(nullptr, nullptr
		, CVector(-40.0f, 20.0f, -10.0f)
		, CVector(-40.0f, -1.0f, 100.0f)
		, CVector(-40.0f, -1.0f, -10.0f));
	CreateColliderTriangle(nullptr, nullptr
		, CVector(-40.0f, -1.0f, 100.0f)
		, CVector(-40.0f, 20.0f, -10.0f)
		, CVector(-40.0f, 20.0f, 100.0f));
	//4����(�^��)
	CreateColliderTriangle(nullptr, nullptr
		, CVector(-40.0f, 20.0f, 100.0f)
		, CVector(40.0f, -1.0f, 100.0f)
		, CVector(-40.0f, -1.0f, 100.0f));
	CreateColliderTriangle(nullptr, nullptr
		, CVector(40.0f, -1.0f, 100.0f)
		, CVector(-40.0f, 20.0f, 100.0f)
		, CVector(40.0f, 20.0f, 100.0f));
}

//�t�B�[���h�ɔz�u����Ă���I�u�W�F�N�g�Ƃ̃��C����
bool CMapManager::CollisionRay(const CVector& start, const CVector& end, float* outDistance)
{
	float nearDistance = -1.0f;
	//�ǂƂ̃��C����
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

	//�ǂ𐶐����ă��X�g�ɒǉ�
	CWall* wall = new CWall(mpWallModel, pos, rot, scale);
	mWall.push_back(wall);
}

void CMapManager::CreateFlag(CVector& pos, CVector& rot, CVector& scale, int priority)
{
	if (mpBlueFlagModel == nullptr) return;
	if (mpRedFlagModel == nullptr) return;

	if (priority == 1)	//�D��x��1�̎��t���b�O�𐶐�
	{
		//�t���b�O�𐶐����ă��X�g�ɒǉ�
		CFlag* blueflag = new CFlag(mpBlueFlagModel, pos, rot, scale);
		mFlag.push_back(blueflag);
	}
	else
	{
		//�ԃt���b�O�𐶐����ă��X�g�ɒǉ�
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