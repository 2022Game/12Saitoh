#include "CGameClearScene.h"
#include "CTaskManager.h"
#include "CSceneManager.h"
#include "CSceneBase.h"
#include "CSound.h"

// �R���X�g���N�^
CGameClearScene::CGameClearScene()
{
	// �Q�[���N���A�pUI�̐���
	mpGameClearUI = new CGameClearUI();

	SetEnable(false);
	SetShow(false);
	mpButton = CResourceManager::Load<CSound>("ButtonSE", "Sound\\SE\\button_se.wav");

	mpButton->SetSimultaneousPlayCount(1);
};

// �f�X�g���N�^
CGameClearScene::~CGameClearScene()
{
	SAFE_DELETE(mpGameClearUI);
}

// �X�V����
void CGameClearScene::Update()
{
	if (!CSceneManager::Instance()->IsReady()) return;
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
			mpButton->Play(0.1f, false, 0.0f);
		}
		// �^�C�g���֖߂�Ȃ�΁A�^�C�g���V�[����ǂݍ���
		else if (mpGameClearUI->IsTitle())
		{
			CSceneManager::Instance()->LoadScene(EScene::eTitle);
			mpButton->Play(0.1f, false, 0.0f);
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