#include "CGameScene.h"
#include "Global.h"
#include "CPlayer.h"
#include "CSceneManager.h"

//�G�̊g�k
#define ENEMY_SIZE CVector(0.045, 0.045, 0.045)

CMapManager* gMap = nullptr;
CPlayer* gPlayer = nullptr;
CEnemy* gEnemy = nullptr;
int CGameScene::mCameraFlag = 0;

//�R���X�g���N�^
CGameScene::CGameScene()
	: CSceneBase(EScene::eGame)
	, mpUi(nullptr)
{
	delete gMap;
	gMap = nullptr;
}

//�f�X�g���N�^
CGameScene::~CGameScene()
{
	delete gPlayer;
	gPlayer = nullptr;
	delete mpUi;
	mpUi = nullptr;
}

//�V�[���ǂݍ���
void CGameScene::Load()
{
	gMap = new CMapManager();
	gMap->Init();

	//�w�i����
	AddTask(new CField());

	//�v���C���[����
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

//�V�[���̍X�V����
void CGameScene::Update()
{
	//�f�o�b�O�p(�J�����̐؂�ւ�)
	if (mInput.PullKey('C'))
	{
		mCameraFlag++;
		if (mCameraFlag >= 3)
		{
			mCameraFlag = 0;
		}
	}
	//�v���C���[�����݂��Ȃ���Ώ������Ȃ�
	CPlayer* player = CPlayer::Instance();
	if (player == nullptr) return;
	//�G�����݂��Ȃ���Ώ������Ȃ�
	CEnemy* enemy = CEnemy::Instance();
	if (enemy == nullptr) return;

	//�J�����̃p�����[�^���쐬����
	CVector e, c, u; //���_�A�����_�A�����
	if (mCameraFlag == 0)
	{
		//���_�����߂�
		e = player->Position() + CVector(0.0f, 3.0f, 0.1f) * player->MatrixRotate();
		//�����_�����߂�
		c = player->Position() + CVector(0.0f, 3.0f, 1.0f) * player->MatrixRotate();
		//����������߂�
		u = CVector(0.0f, 0.1f, 0.0f) * player->MatrixRotate();
		//�J�����̐ݒ�
		gluLookAt(e.X(), e.Y(), e.Z(), c.X(), c.Y(), c.Z(), u.X(), u.Y(), u.Z());
	}
	else if (mCameraFlag == 1)
	{
		//���_�����߂�
		e = player->Position() + CVector(-0.9f, 3.0f, -12.0f) * player->MatrixRotate();
		//�����_�����߂�
		c = player->Position();
		//����������߂�
		u = CVector(0.0f, 1.0f, 0.0f) * player->MatrixRotate();
		//�J�����̐ݒ�
		gluLookAt(e.X(), e.Y(), e.Z(), c.X(), c.Y(), c.Z(), u.X(), u.Y(), u.Z());
	}
	else
	{
		//���_�����߂�
		e = player->Position() + CVector(0.5f, 3.0f, 12.0f) * player->MatrixRotate();
		//�����_�����߂�
		c = player->Position();
		//����������߂�
		u = CVector(0.0f, 1.0f, 0.0f) * player->MatrixRotate();
		//�J�����̐ݒ�
		gluLookAt(e.X(), e.Y(), e.Z(), c.X(), c.Y(), c.Z(), u.X(), u.Y(), u.Z());
	}

	//�v���C���[�����S������Q�[���I�[�o�[��ʂɈڍs����
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
	//�G�̐���0�ɂȂ�����Q�[���N���A��ʂɈڍs����
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