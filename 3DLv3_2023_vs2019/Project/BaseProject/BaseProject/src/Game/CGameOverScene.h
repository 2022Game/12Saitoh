#pragma once
#include "CResultScene.h"
#include "CImage.h"
#include "CLightingButton.h"
#include "CGameOverUI.h"
class CSound;

// �Q�[���I�[�o�[�V�[��
class CGameOverScene : public CResultScene 
{
public:
	// �R���X�g���N�^ 
	CGameOverScene();
	// �f�X�g���N�^
	~CGameOverScene();

	// �X�V����
	void Update();
	// �`�揈��
	void Render();
	// �Q�[���I�[�o�[�J�n
	void Start();
	// �Q�[���I�[�o�[�I��
	void End();

private:
	CGameOverUI* mpGameOverUI;
	CSound* mpButton;// �{�^��SE
};