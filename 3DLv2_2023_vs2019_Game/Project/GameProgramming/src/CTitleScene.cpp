#include "CTitleScene.h"
#include "main.h"
#include "CSceneManager.h"

//�R���X�g���N�^
CTitleScene::CTitleScene()
	: CSceneBase(EScene::eTitle)
	, mBgImage(nullptr)
	, mpUi(nullptr)
{

}

//�f�X�g���N�^
CTitleScene::~CTitleScene()
{
	delete mBgImage;
	mBgImage = nullptr;
	delete mpUi;
	mpUi = nullptr;
}

//�V�[���ǂݍ���
void CTitleScene::Load()
{
	mBgImage = new CImage("white.png");
	mBgImage->SetColor(0.0f, 0.0f, 0.0f, 1.0f);
	mBgImage->SetPos(0.0f, 0.0f);
	mBgImage->SetSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	mpUi = new CUi();
}

//�V�[���̍X�V����
void CTitleScene::Update()
{
	if (mInput.PushKey(VK_RETURN))
	{
		CSceneManager::Instance()->LoadScene(EScene::eGame);
	}
	else
	{
		if (mBgImage != nullptr)
		{
			mBgImage->Render();
		}
	}
}