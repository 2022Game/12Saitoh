#pragma once
#include "CSceneBase.h"
#include "CInput.h"
#include "CImage.h"
#include "CUi.h"

//リザルトシーン
class COverScene : public CSceneBase
{
public:
	//コンストラクタ
	COverScene();
	//デストラクタ
	~COverScene();

	//シーンの読み込み
	void Load();
	//シーンの更新処理
	void Update();
private:
	CInput mInput;		//入力判定
	CImage* mpBgImage;	//背景画像
	CUi* mpUi;
};