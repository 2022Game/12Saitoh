#include "COverScene.h"
#include "main.h"
#include "CSceneManager.h"
#include "Cplayer.h"

//�R���X�g���N�^
COverScene::COverScene()
	: CSceneBase(EScene::eOver)
	, mpBgImage(nullptr)
	, mpUi(nullptr)
{

}

//�f�X�g���N�^
COverScene::~COverScene()
{
	delete mpBgImage;
	mpBgImage = nullptr;
	delete mpUi;
	mpUi = nullptr;
}

//�V�[���̓ǂݍ���
void COverScene::Load()
{
	mpBgImage = new CImage("white.png");
	mpBgImage->SetColor(0.0f, 0.0f, 0.0f, 1.0f);
	mpBgImage->SetPos(0.0f, 0.0f);
	mpBgImage->SetSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	mpUi = new CUi();
}

//�V�[���̍X�V����
void COverScene::Update()
{
	if (mInput.PushKey(VK_RETURN))
	{
		CSceneManager::Instance()->LoadScene(EScene::eTitle);
	}
	else
	{
		if (mpBgImage != nullptr)
		{
			mpBgImage->Render();
		}
	}
}