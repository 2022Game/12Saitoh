#pragma once
#include "CUi.h"
#include "CEnemy.h"
#define TEXTURE "rafuimage.png" //テクスチャのファイル名
#define BACKTEXTURE "backimage.png" //背景テクスチャのファイル名
#define TIPSIZE 50
//定数の定義
/*
* CGameクラス
* ゲームクラス
*/
class CGame
{
public:
	//デストラクタ
	~CGame();
	//ゲームクリア判定
	//bool IsClear();
	//ゲームクリア処理
	void Clear();
	//ゲームオーバー判定
	//bool IsOver();
	//ゲームオーバー処理
	void Over();
	//スタート処理
	void Start();
	CGame(); //デフォルトコンストラクタ（初期処理）
	void Update(); //更新処理
private:
	CEnemy* mpEnemy;
	CEnemy* mpEnemy2;
	int mCdx, mCdy; //カメラとプレイヤーの座標の差分
	void CameraSet(); //カメラ設定
	int mTime; //経過時間
	CUi *mpUi; //UIクラスのポインタ
};
