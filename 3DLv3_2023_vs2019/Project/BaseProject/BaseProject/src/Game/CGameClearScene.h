#pragma once
#include "CResultScene.h"
#include "CImage.h"
#include "CLightingButton.h"
#include "CGameClearUI.h"
class CSound;

// ゲームオーバーシーン
class CGameClearScene : public CResultScene
{
public:
	// コンストラクタ 
	CGameClearScene();
	// デストラクタ
	~CGameClearScene();

	// 更新処理
	void Update();
	// 描画処理
	void Render();
	// ゲームオーバー開始
	void Start();
	// ゲームオーバー終了
	void End();

private:
	CGameClearUI* mpGameClearUI;
	CSound* mpButton;// ボタンSE
};