#pragma once
#include "CResultScene.h"
#include "CImage.h"
#include "CLightingButton.h"
#include "CGameOverUI.h"

// ゲームオーバーシーン
class CGameOverScene : public CResultScene 
{
public:
	// コンストラクタ 
	CGameOverScene();
	// デストラクタ
	~CGameOverScene();

	// 更新処理
	void Update();
	// 描画処理
	void Render();
	// ゲームオーバー開始
	void Start();
	// ゲームオーバー終了
	void End();

private:
	CGameOverUI* mpGameOverUI;
};