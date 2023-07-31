#pragma once
#include "CSceneBase.h"
#include "CInput.h"
#include "CImage.h"
#include "CUi.h"

//�^�C�g���V�[��
class CTitleScene : public CSceneBase
{
public:
	//�R���X�g���N�^
	CTitleScene();
	//�f�X�g���N�^
	~CTitleScene();

	//�V�[���ǂݍ���
	void Load();
	//�V�[���̍X�V����
	void Update();

private:
	CInput mInput;		//���͔���
	CImage* mBgImage;	//�w�i�摜
	CUi* mpUi;
};