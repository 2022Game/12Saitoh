#include "CTitleScene.h"
#include "CSceneManager.h"
#include "CGameScene.h"
#include "CSound.h"
#include "CBGMManager.h"
#include "CTitleUI.h"
#include "CModel.h"
#include "CTitleCamera.h"
#include "CTitleField.h"
#include "CTitleModel.h"

//�R���X�g���N�^
CTitleScene::CTitleScene()
	: CSceneBase(EScene::eTitle)
{
}

//�f�X�g���N�^
CTitleScene::~CTitleScene()
{
}

//�V�[���ǂݍ���
void CTitleScene::Load()
{
	// �^�C�g����ʂ̓J�[�\���\��
	CInput::ShowCursor(true);
	// �w�i�F�ݒ�
	//System::SetClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	// �w�i�F�ݒ�
	System::SetClearColor(0.1921569f, 0.3019608f, 0.4745098f, 1.0f);

	//�����Ń^�C�g����ʂɕK�v��
	//���\�[�X�̓ǂݍ��݂�N���X�̐������s��
	CResourceManager::Load<CModel>("TitleField", "Field\\WaitingRoom\\WaitingRoom.obj");
	CResourceManager::Load<CModelX>("TitleModel", "Character\\Player\\player.x");
	CResourceManager::Load<CModel>("Sword", "Character\\Sword\\sword.obj");

	// �^�C�g���p�̃X�e�[�W�𐶐�
	CTitleField* titleField = new CTitleField();
	titleField->Scale(100.0f, 100.0f, 100.0f);
	titleField->Position(0.0f, -30.0f, 0.0f);

	// �^�C�g���p�̃v���C���[���f���̐���
	CTitleModel* titleModel = new CTitleModel();
	titleModel->Scale(80.0f, 80.0f, 80.0f);
	titleModel->Rotate(0.0f, 180.0f, 0.0f);
	titleModel->Position(0.0f, 10.0f, 0.0f);

	CVector tPos = titleModel->Position();
	CTitleCamera* titleCamera = new CTitleCamera
	(
		tPos + CVector(25.0f, 108.0f, -70.0f),
		tPos + CVector(30.0f, 108.0f, 0.0f)
	);

	//���C���J�����̒Ǐ]�^�[�Q�b�g���v���C���[�ɐݒ�
	//titleCamera->SetFollowTargetTf(player);

	// �^�C�g��BGM���Đ�
	CBGMManager::Instance()->Play(EBGMType::eTitle);

	//mBgImage = new CImage("white.png");
	//mBgImage->SetColor(0.0f, 0.0f, 0.0f, 1.0f);
	//mBgImage->SetPos(0.0f, 0.0f);
	//mBgImage->SetSize(WINDOW_WIDTH, WINDOW_HEIGHT);

	//CCamera* mainCamera = new CCamera
	//(
	//	CVector(0.0f, 50.0f, 75.0f),
	//	CVector::zero
	//);

	mpTitleUI = new CTitleUI();
	AddTask(mpTitleUI);
}

//�V�[���̍X�V����
void CTitleScene::Update()
{
	// �^�C�g����ʂ��I��
	if (mpTitleUI->IsEnd())
	{
		// �Q�[���J�n�Ȃ�΁A�Q�[���V�[����ǂݍ���
		if (mpTitleUI->IsStartGame())
		{
			CSceneManager::Instance()->LoadScene(EScene::eGame);
		}
		// �Q�[���I���Ȃ�΁A�A�v�������
		else if (mpTitleUI->IsExitGame())
		{
			System::ExitGame();
		}
	}
}
