#include "CGameOverScene.h"
#include "CTaskManager.h"
#include "CSceneManager.h"
#include "CSceneBase.h"

// �R���X�g���N�^
CGameOverScene::CGameOverScene()
{
	// �Q�[���I�[�o�[�pUI�̐���
	mpGameOverUI = new CGameOverUI();

	SetEnable(false);
	SetShow(false);
};

// �f�X�g���N�^
CGameOverScene::~CGameOverScene()
{
	SAFE_DELETE(mpGameOverUI);
}

// �X�V����
void CGameOverScene::Update()
{
	// �x�[�X�N���X�̍X�V
	CResultScene::Update();
	// UI�̍X�V
	mpGameOverUI->Update();

	// �Q�[���I�[�o�[��ʂ��I��
	if (mpGameOverUI->IsEnd())
	{
		// ���g���C�Ȃ�΁A�Q�[���V�[����ǂݍ���
		if (mpGameOverUI->IsReTry())
		{
			CSceneManager::Instance()->LoadScene(EScene::eGame);
		}
		// �^�C�g���֖߂�Ȃ�΁A�^�C�g���V�[����ǂݍ���
		else if (mpGameOverUI->IsTitle())
		{
			CSceneManager::Instance()->LoadScene(EScene::eTitle);
		}
	}
}

// �`�揈��
void CGameOverScene::Render()
{
	// �x�[�X�N���X�̕`�揈��
	CResultScene::Render();
	// UI�̕`�揈��
	mpGameOverUI->Render();
}

// �Q�[���I�[�o�[�J�n
void CGameOverScene::Start()
{
	CResultScene::Start();
	mpGameOverUI->Start();
}

void CGameOverScene::End()
{
	CResultScene::End();
	mpGameOverUI->End();
}