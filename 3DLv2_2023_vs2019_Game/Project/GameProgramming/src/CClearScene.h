#pragma once
#include "CSceneBase.h"
#include "CInput.h"
#include "CImage.h"
#include "CUi.h"

//リザルトシーン
class CClearScene : public CSceneBase
{
public:
	//コンストラクタ
	CClearScene();
	//デストラクタ
	~CClearScene();

	//シーンの読み込み
	void Load();
	//シーンの更新処理
	void Update();
private:
	CInput mInput;		//入力判定
	CImage* mpBgImage;	//背景画像
	CUi* mpUi;
};