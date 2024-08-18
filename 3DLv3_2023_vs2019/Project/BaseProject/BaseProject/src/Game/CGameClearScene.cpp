#include "CGameClearScene.h"
#include "CTaskManager.h"
#include "CSceneManager.h"
#include "CSceneBase.h"

// �R���X�g���N�^
CGameClearScene::CGameClearScene()
{
	// �Q�[���N���A�pUI�̐���
	mpGameClearUI = new CGameClearUI();

	SetEnable(false);
	SetShow(false);
};

// �f�X�g���N�^
CGameClearScene::~CGameClearScene()
{
	SAFE_DELETE(mpGameClearUI);
}

// �X�V����
void CGameClearScene::Update()
{
	// �x�[�X�N���X�̍X�V
	CResultScene::Update();
	// UI�̍X�V
	mpGameClearUI->Update();

	// �Q�[���N���A��ʂ��I��
	if (mpGameClearUI->IsEnd())
	{
		// ���g���C�Ȃ�΁A�Q�[���V�[����ǂݍ���
		if (mpGameClearUI->IsReTry())
		{
			CSceneManager::Instance()->LoadScene(EScene::eGame);
		}
		// �^�C�g���֖߂�Ȃ�΁A�^�C�g���V�[����ǂݍ���
		else if (mpGameClearUI->IsTitle())
		{
			CSceneManager::Instance()->LoadScene(EScene::eTitle);
		}
	}
}

// �`�揈��
void CGameClearScene::Render()
{
	// �x�[�X�N���X�̕`�揈��
	CResultScene::Render();
	// UI�̕`�揈��
	mpGameClearUI->Render();
}

// �Q�[���N���A�J�n
void CGameClearScene::Start()
{
	CResultScene::Start();
	mpGameClearUI->Start();
}

// �Q�[���N���A�I��
void CGameClearScene::End()
{
	CResultScene::End();
	mpGameClearUI->End();
}