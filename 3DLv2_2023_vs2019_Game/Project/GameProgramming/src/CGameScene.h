#pragma once
#include "CSceneBase.h"
#include "CField.h"
#include "CInput.h"
#include "CUi.h"

//�^�C�g���V�[��
class CGameScene : public CSceneBase
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
	static int mCameraFlag; //�f�o�b�O�p

	CInput mInput;
	CUi* mpUi;
};