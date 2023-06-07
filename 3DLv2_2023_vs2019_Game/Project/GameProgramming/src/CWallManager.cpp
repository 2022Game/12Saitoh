#include "CWallManager.h"
#include "CTaskManager.h"

//�y��Coin
#define MODEL_WALL "res\\Wall.obj","res\\Wall.mtl"

CWallManager::CWallManager()
{
	//��������
	Init();

	// �^�X�N�}�l�[�W���[�Ɏ��g��ǉ�
	CTaskManager::Instance()->Add(this);
}

CWallManager::~CWallManager()
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
	//�ꉞ���X�g���N���A���Ă���
	mWall.clear();

	//�^�X�N�}�l�[�W���[���玩�g����菜��
	CTaskManager::Instance()->Remove(this);
}

void CWallManager::Init()
{
	//�ǂ̃��f���ǂݍ���
	mpWallModel = new CModel();
	mpWallModel->Load(MODEL_WALL);

	//�ǂ𐶐�
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

	//�ǂ𐶐����ă��X�g�ɒǉ�
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