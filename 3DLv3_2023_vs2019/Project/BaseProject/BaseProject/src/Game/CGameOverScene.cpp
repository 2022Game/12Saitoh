#include "CGameOverScene.h"
#include "CTaskManager.h"
#include "CSceneManager.h"
#include "CSceneBase.h"
#include "CSound.h"

// �R���X�g���N�^
CGameOverScene::CGameOverScene()
{
	// �Q�[���I�[�o�[�pUI�̐���
	mpGameOverUI = new CGameOverUI();

	SetEnable(false);
	SetShow(false);
	mpButton = CResourceManager::Load<CSound>("ButtonSE", "Sound\\SE\\button_se.wav");
};

// �f�X�g���N�^
CGameOverScene::~CGameOverScene()
{
	SAFE_DELETE(mpGameOverUI);
}

// �X�V����
void CGameOverScene::Update()
{
	if (!CSceneManager::Instance()->IsReady()) return;
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
			mpButton->Play(0.1f, false, 0.0f);
		}
		// �^�C�g���֖߂�Ȃ�΁A�^�C�g���V�[����ǂݍ���
		else if (mpGameOverUI->IsTitle())
		{
			CSceneManager::Instance()->LoadScene(EScene::eTitle);
			mpButton->Play(0.1f, false, 0.0f);
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