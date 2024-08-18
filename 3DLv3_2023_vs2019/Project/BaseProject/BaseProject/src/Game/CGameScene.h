#pragma once
#include "CSceneBase.h"
class CGameMenu;
class CSound;
class CGameOverScene;
class CGameClearScene;

//�^�C�g���V�[��
class CGameScene :public CSceneBase
{
public:
	//�R���X�g���N�^
	CGameScene();
	//�f�X�g���N�^
	~CGameScene();
	//�V�[���ǂݍ���
	void Load();
	//�V�[���̍X�V����
	void Update();

private:
	CGameMenu* mpGameMenu;
	CGameOverScene* mpGameOver;
	CGameClearScene* mpGameClear;
};
