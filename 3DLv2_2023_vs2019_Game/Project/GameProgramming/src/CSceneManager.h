#pragma once
#include "CSceneBase.h"
#include "SceneType.h"

class CSceneManager
{
public:
	//インスタンスを取得
	static CSceneManager* Instance();
	//インスタンスを破棄
	static void ClearInstance();
	//シーン読み込み
	void LoadScene(EScene scene);
	//読み込んでいるシーンを破棄
	void UnloadScene();
	//読み込んでいるシーンの更新処理
	void Update();
	//SceneTypeの取得
	int Scene();
private:
	//コンストラクタ
	CSceneManager();
	//デストラクタ
	~CSceneManager();

	//CSceneManagerのインスタンス
	static CSceneManager* mpInstance;
	//読み込んでいるシーン
	CSceneBase* mpScene;
	//SceneType
	EScene mScene;
};