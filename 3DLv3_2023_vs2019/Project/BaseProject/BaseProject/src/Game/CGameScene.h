#pragma once
#include "CSceneBase.h"
class CGameMenu;
class CSound;
class CGameOverScene;
class CGameClearScene;
class CBGMManager;

//タイトルシーン
class CGameScene :public CSceneBase
{
public:
	//コンストラクタ
	CGameScene();
	//デストラクタ
	~CGameScene();
	//シーン読み込み
	void Load();
	//シーンの更新処理
	void Update();

private:
	CGameMenu* mpGameMenu;
	CGameOverScene* mpGameOver;
	CGameClearScene* mpGameClear;
	CSound* mpGameBGM;		// ゲーム中BGM
	CSound* mpGameBGM2;		// ゲーム中のBGM
	CSound* mpNature;		// 環境音
};
