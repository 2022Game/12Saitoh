#pragma once
#include "CSceneBase.h"
#include "SceneType.h"

class CSceneManager
{
public:
	//�C���X�^���X���擾
	static CSceneManager* Instance();
	//�C���X�^���X��j��
	static void ClearInstance();
	//�V�[���ǂݍ���
	void LoadScene(EScene scene);
	//�ǂݍ���ł���V�[����j��
	void UnloadScene();
	//�ǂݍ���ł���V�[���̍X�V����
	void Update();
	//SceneType�̎擾
	int Scene();
private:
	//�R���X�g���N�^
	CSceneManager();
	//�f�X�g���N�^
	~CSceneManager();

	//CSceneManager�̃C���X�^���X
	static CSceneManager* mpInstance;
	//�ǂݍ���ł���V�[��
	CSceneBase* mpScene;
	//SceneType
	EScene mScene;
};