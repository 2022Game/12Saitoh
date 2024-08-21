#pragma once
#include "CResultScene.h"
#include "CImage.h"
#include "CLightingButton.h"
#include "CGameClearUI.h"
class CSound;

// �Q�[���I�[�o�[�V�[��
class CGameClearScene : public CResultScene
{
public:
	// �R���X�g���N�^ 
	CGameClearScene();
	// �f�X�g���N�^
	~CGameClearScene();

	// �X�V����
	void Update();
	// �`�揈��
	void Render();
	// �Q�[���I�[�o�[�J�n
	void Start();
	// �Q�[���I�[�o�[�I��
	void End();

private:
	CGameClearUI* mpGameClearUI;
	CSound* mpButton;// �{�^��SE
};