#pragma once
#include "CResultUIBase.h"

class CGameClearUI : public CResultUIBase
{
public:
	// コンストラクタ
	CGameClearUI();
	// デストラクタ
	~CGameClearUI();

	// 更新処理
	void Update() override;
	// 描画処理
	void Render() override;

	// UI処理開始
	void Start() override;
	// UI処理終了
	void End() override;
};