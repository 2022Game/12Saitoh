#include "CSceneManager.h"
#include "CTitleScene.h"
#include "CGameScene.h"
#include "COverScene.h"
#include "CClearScene.h"

CSceneManager* CSceneManager::mpInstance = nullptr;

//�C���X�^���X���擾
CSceneManager* CSceneManager::Instance()
{
	//�C���X�^���X�����݂��Ȃ��ꍇ�͐V������������
	if (mpInstance == nullptr)
	{
		mpInstance = new CSceneManager();
	}
	return mpInstance;
}

//�C���X�^���X��j��
void CSceneManager::ClearInstance()
{
	delete mpInstance;
	mpInstance = nullptr;
}

//�V�[���ǂݍ���
void CSceneManager::LoadScene(EScene scene)
{
	//���ɃV�[�����ǂݍ��܂�ċ�����A
	//���̃V�[����j��
	UnloadScene();

	//�ǂݍ��ރV�[���̎�ނɍ��킹��
	//��������V�[���̃N���X��ύX����
	switch (scene)
	{
		//�^�C�g���V�[��
	case EScene::eTitle:
		mpScene = new CTitleScene();
		break;
		//�Q�[���V�[��
	case EScene::eGame:
		mpScene = new CGameScene();
		break;
		//�Q�[���I�[�o�[�V�[��
	case EScene::eOver:
		mpScene = new COverScene();
		break;
	case EScene::eClear:
		mpScene = new CClearScene();
		break;
	}

	//�V�[����V��������������ǂݍ��݊J�n
	if (mpScene != nullptr)
	{
		mpScene->Load();
	}
	mScene = scene;
}

//�ǂݍ���ł���V�[����j��
void CSceneManager::UnloadScene()
{
	if (mpScene != nullptr)
	{
		delete mpScene;
		mpScene = nullptr;
	}
}

//�ǂݍ���ł���V�[���̍X�V����
void CSceneManager::Update()
{
	//�V�[�����ǂݍ��܂�Ă��Ȃ��ꍇ�́A�������Ȃ�
	if (mpScene == nullptr) return;
	mpScene->Update();
}

//�R���X�g���N�^
CSceneManager::CSceneManager()
	: mpScene(nullptr)
	, mScene(EScene::eNone)
{

}

//�f�X�g���N�^
CSceneManager::~CSceneManager()
{
	//�ǂݍ���ł���V�[�����L��΁A�j������
	UnloadScene();
}

int CSceneManager::Scene()
{
	return (int)mScene;
}