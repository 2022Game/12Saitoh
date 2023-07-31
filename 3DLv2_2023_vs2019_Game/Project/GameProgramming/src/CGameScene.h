#pragma once
#include "CSceneBase.h"
#include "CField.h"
#include "CInput.h"
#include "CUi.h"

//タイトルシーン
class CGameScene : public CSceneBase
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
	static int mCameraFlag; //デバッグ用

	CInput mInput;
	CUi* mpUi;
};