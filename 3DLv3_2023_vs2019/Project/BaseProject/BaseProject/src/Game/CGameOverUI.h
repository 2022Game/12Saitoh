#pragma once
#include "CResultUIBase.h"

class CGameOverUI : public CResultUIBase
{
public:
	// コンストラクタ
	CGameOverUI();
	// デストラクタ
	~CGameOverUI();

	// 更新処理
	void Update() override;
	// 描画処理
	void Render() override;

	// UI処理開始
	void Start() override;
	// UI処理終了
	void End() override;
private:
};