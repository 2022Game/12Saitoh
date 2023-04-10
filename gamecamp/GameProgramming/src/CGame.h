#pragma once
#include "CUi.h"
#include "CPlayer.h"
#include "CEnemy.h"
#include "CEnemy2.h"
#include "CBackGround.h"
#include "CBackGround2.h"
#include "CBackGround3.h"
#include "CBackGround4.h"
#include "CHeart.h"
#include "CInput.h"
#include "CSlime.h"
#include "CWolf.h"
#include "CBoss.h"
#include "CItem.h"
#include "CBlock.h"

//定数の定義
#define TEXTURE "image.png"//テクスチャのファイル名
#define TIPSIZE 40.0f //基本サイズ
class CGame
{
private:
	CPlayer* mpPlayer; //プレイヤーのポインタ
	CEnemy* mpEnemy;   //敵のポインタ
	CEnemy2* mpEnemy2; //敵２のポインタ
	CSlime* mpSlime;
	CWolf* mpWolf;
	CBoss* mpBoss;
	CItem* mpItem;
	CBlock* mpBlock;
	CBackGround* mpBackGround;
	CBackGround2* mpBackGround2;
	CBackGround3* mpBackGround3;
	CBackGround4* mpBackGround4;
	CHeart* mpHeart;
	CInput mInput;
	CUi* mpUi;

	int mCdx, mCdy;      //カメラとプレイヤーの座標の差分
	int mTime;           //経過時間
	static int sNum;     //敵の数
	static int sItemNum; //アイテムの数

public:
	//アイテムの数を取得
	static int ItemuNum();
	//敵の数を取得
	static int Num();

	//コンストラクタ
	CGame();
	//デストラクタ
	~CGame();

	//ゲームクリア処理
	void Clear();

	//ゲームオーバー処理
	void Over();

	//スタート処理
	void Start();
	//ステージ
	void Stage1();
	void Stage2();
	void Boss();
	void SetItem();
	//更新処理
	void Update();
};
