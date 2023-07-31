#pragma once
#include "CSceneBase.h"
#include "CInput.h"
#include "CImage.h"
#include "CUi.h"

//タイトルシーン
class CTitleScene : public CSceneBase
{
public:
	//コンストラクタ
	CTitleScene();
	//デストラクタ
	~CTitleScene();

	//シーン読み込み
	void Load();
	//シーンの更新処理
	void Update();

private:
	CInput mInput;		//入力判定
	CImage* mBgImage;	//背景画像
	CUi* mpUi;
};