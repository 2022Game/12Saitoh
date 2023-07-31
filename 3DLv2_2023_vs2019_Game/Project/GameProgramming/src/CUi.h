#pragma once
#include "CFont.h"
#include "CTask.h"
#include "CSceneManager.h"
/*
* CUi
* ユーザーインタフェースクラス
*/
class CUi : public CTask
{
private:
	int Scene();//ゲームシーンの取得
	int mScene;	//シーン
	int mTime;	//経過時間
	CFont mFont;

public:
	//コンストラクタ
	CUi();
	//デストラクタ
	~CUi();

	//スタート表示
	void Title();
	//ゲーム画面
	void Game();
	//ゲームオーバー表示
	void Over();
	//ゲームクリア表示
	void Clear();
	//描画処理
	void Render();

	//経過時間の取得
	void Time(int time);
};
