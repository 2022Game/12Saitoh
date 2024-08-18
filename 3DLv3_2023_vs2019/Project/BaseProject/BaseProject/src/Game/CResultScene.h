#pragma once
#include "CTask.h"
#include "CImage.h"
#include "CSceneBase.h"

// リザルトシーンのベースクラス
class CResultScene : public CTask 

{
public:
	// コンストラクタ
	CResultScene();
	// デストラクタ
	~CResultScene();

	// 更新処理
	void Update() override;
	// 描画処理
	void Render() override;

	// 開始処理
	void Start();
	// 終了処理
	void End();

	// リザルトシーン再生中かどうか取得
	bool IsPlayResult();
private:
	// リザルトシーン再生中かどうか
	bool mIsPlay;
	// リザルトシーンの段階
	int mResultStep;
	// 経過時間計測長
	float mElapsedTime;

	// リザルト用背景イメージ
	CImage* mpBackImage;
};