#include "CMapManager.h"
#include "CTaskManager.h"

#define MODEL_WALL "res\\Wall.obj","res\\Wall.mtl"				//��
#define MODEL_BLUEFLAG "res\\BlueFlag.obj","res\\BlueFlag.mtl"	//�t���b�O
#define MODEL_REDFLAG "res\\RedFlag.obj","res\\RedFlag.mtl"		//�ԃt���b�O
#define WALL_SIZE CVector(1.0f, 1.5f, 1.2f)						//�ǂ̃T�C�Y
#define FLAG_SIZE CVector(0.2f, 0.2f, 0.2f)						//�t���b�O�̃T�C�Y

CMapManager::CMapManager()
{
	//��������
	Init();

	// �^�X�N�}�l�[�W���[�Ɏ��g��ǉ�
	CTaskManager::Instance()->Add(this);
}

CMapManager::~CMapManager()
{
	//�ǂ̃��f���f�[�^��j��
	if (mpWallModel != nullptr)
	{
		delete mpWallModel;
	}
	//�ǂ�j��
	for (int i = 0; i < mWall.size(); i++)
	{
		delete mWall[i];
	}
	//�t���b�O�̃��f���f�[�^��j��
	if (mpBlueFlagModel != nullptr)
	{
		delete mpBlueFlagModel;
	}
	//�ԃt���b�O�̃��f���f�[�^��j��
	if (mpRedFlagModel != nullptr)
	{
		delete mpRedFlagModel;
	}
	//�t���b�O��j��
	for (int i = 0; i < mFlag.size(); i++)
	{
		delete mFlag[i];
	}

	//�ꉞ���X�g���N���A���Ă���
	mWall.clear();
	mFlag.clear();

	//�^�X�N�}�l�[�W���[���玩�g����菜��
	CTaskManager::Instance()->Remove(this);
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
	CreateWall(CVector(0.0f, 1.0f, 5.0f), CVector(0.0f,90.0f,0.0f), WALL_SIZE);
	CreateWall(CVector(-5.0f, 1.0f, 10.0f), CVector(0.0f,90.0f,0.0f), WALL_SIZE);
	CreateWall(CVector(7.0f, 1.0f, 15.0f), CVector(0.0f, 90.0f, 0.0f), WALL_SIZE);
	CreateWall(CVector(0.0f, 1.0f, 20.0f), CVector(0.0f, 90.0f, 0.0f), WALL_SIZE);
	CreateWall(CVector(-5.0f, 1.0f, 25.0f), CVector(0.0f, 90.0f, 0.0f), WALL_SIZE);
	CreateWall(CVector(7.0f, 1.0f, 30.0f), CVector(0.0f, 90.0f, 0.0f), WALL_SIZE);
	CreateWall(CVector(0.0f, 1.0f, 35.0f), CVector(0.0f, 90.0f, 0.0f), WALL_SIZE);
	//�t���b�O�𐶐�
	//CreateFlag(CVector(0.0f, -1.0f, 10.5f), CVector(), FLAG_SIZE, 1);
	// CreateFlag(CVector(0.0f, -1.0f, 29.5f), CVector(0.0f, 180.0f, 0.0f), FLAG_SIZE, 0);
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