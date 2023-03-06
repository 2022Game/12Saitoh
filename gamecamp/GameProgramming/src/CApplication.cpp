#include "CApplication.h"
#include "CRectangle.h"

void CApplication::Start()
{
	mpGame = new CGame();
}

void CApplication::Update()
{
	mCharacterManager.Update();
	mCharacterManager.Delete();
	mCharacterManager.Render();
}

CApplication::~CApplication()
{
	delete spUi; //インスタンスUiの削除
}

CUi* CApplication::spUi = nullptr;
CUi* CApplication::Ui()
{
	return spUi; //インスタンスのポインタを返す
}

CCharacterManager CApplication::mCharacterManager;
CCharacterManager* CApplication::CharacterManager()
{
	return &mCharacterManager;
}

CTexture CApplication::mTexture;
CTexture* CApplication::Texture()
{
	return &mTexture;
}
CTexture CApplication::mTexture2;
CTexture* CApplication::Texture2()
{
	return &mTexture2;
}