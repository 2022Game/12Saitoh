#pragma once
#include "CSceneBase.h"
class CGameMenu;
class CSound;
class CGameOverScene;
class CGameClearScene;
class CBGMManager;

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
	float mElapsedTime;

	CGameMenu* mpGameMenu;
	CGameOverScene* mpGameOver;
	CGameClearScene* mpGameClear;
	CSound* mpGameBGM;		// �Q�[����BGM
	CSound* mpGameBGM2;		// �Q�[������BGM
	CSound* mpGameClearBGM;	// �Q�[���N���A����BGM
	CSound* mpGameOverBGM;	// �Q�[���I�[�o�[����BGM
	CSound* mpNature;		// ����
};
