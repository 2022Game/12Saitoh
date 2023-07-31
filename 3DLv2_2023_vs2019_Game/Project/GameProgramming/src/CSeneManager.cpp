#include "CSceneManager.h"
#include "CTitleScene.h"
#include "CGameScene.h"
#include "COverScene.h"
#include "CClearScene.h"

CSceneManager* CSceneManager::mpInstance = nullptr;

//インスタンスを取得
CSceneManager* CSceneManager::Instance()
{
	//インスタンスが存在しない場合は新しく生成する
	if (mpInstance == nullptr)
	{
		mpInstance = new CSceneManager();
	}
	return mpInstance;
}

//インスタンスを破棄
void CSceneManager::ClearInstance()
{
	delete mpInstance;
	mpInstance = nullptr;
}

//シーン読み込み
void CSceneManager::LoadScene(EScene scene)
{
	//既にシーンが読み込まれて居たら、
	//そのシーンを破棄
	UnloadScene();

	//読み込むシーンの種類に合わせて
	//生成するシーンのクラスを変更する
	switch (scene)
	{
		//タイトルシーン
	case EScene::eTitle:
		mpScene = new CTitleScene();
		break;
		//ゲームシーン
	case EScene::eGame:
		mpScene = new CGameScene();
		break;
		//ゲームオーバーシーン
	case EScene::eOver:
		mpScene = new COverScene();
		break;
	case EScene::eClear:
		mpScene = new CClearScene();
		break;
	}

	//シーンを新しく生成したら読み込み開始
	if (mpScene != nullptr)
	{
		mpScene->Load();
	}
	mScene = scene;
}

//読み込んでいるシーンを破棄
void CSceneManager::UnloadScene()
{
	if (mpScene != nullptr)
	{
		delete mpScene;
		mpScene = nullptr;
	}
}

//読み込んでいるシーンの更新処理
void CSceneManager::Update()
{
	//シーンが読み込まれていない場合は、処理しない
	if (mpScene == nullptr) return;
	mpScene->Update();
}

//コンストラクタ
CSceneManager::CSceneManager()
	: mpScene(nullptr)
	, mScene(EScene::eNone)
{

}

//デストラクタ
CSceneManager::~CSceneManager()
{
	//読み込んでいるシーンが有れば、破棄する
	UnloadScene();
}

int CSceneManager::Scene()
{
	return (int)mScene;
}