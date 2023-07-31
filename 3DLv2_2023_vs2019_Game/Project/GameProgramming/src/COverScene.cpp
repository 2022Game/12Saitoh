#include "COverScene.h"
#include "main.h"
#include "CSceneManager.h"
#include "Cplayer.h"

//コンストラクタ
COverScene::COverScene()
	: CSceneBase(EScene::eOver)
	, mpBgImage(nullptr)
	, mpUi(nullptr)
{

}

//デストラクタ
COverScene::~COverScene()
{
	delete mpBgImage;
	mpBgImage = nullptr;
	delete mpUi;
	mpUi = nullptr;
}

//シーンの読み込み
void COverScene::Load()
{
	mpBgImage = new CImage("white.png");
	mpBgImage->SetColor(0.0f, 0.0f, 0.0f, 1.0f);
	mpBgImage->SetPos(0.0f, 0.0f);
	mpBgImage->SetSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	mpUi = new CUi();
}

//シーンの更新処理
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