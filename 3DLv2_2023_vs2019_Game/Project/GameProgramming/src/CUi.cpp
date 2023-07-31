#include <stdio.h>
#include "CUi.h"
#include "CCamera.h"
#include "CEnemy.h"
#include "CPlayer.h"

//ゲームシーンの取得
int CUi::Scene()
{
	return mScene = CSceneManager::Instance()->Scene();
}

CUi::CUi()
    : mScene(0)
	, mTime(0)
{
	//ポーズの種類を設定
	SetPauseType(TaskPauseType::eGame);
	mFont.Load("FontWhite.png", 1, 64);
	mPriority = (int)TaskPriority::eUI;
	//タスクマネージャに自信を追加
	CTaskManager::Instance()->Add(this);
}

//デストラクタ
CUi::~CUi()
{
	CTaskManager::Instance()->Remove(this);
}

//スタート画面
void CUi::Title()
{
	CCamera::Start(0, 800, 0, 600); //2D描画開始
	mFont.Draw(36, 300, 18, 36, "START PUSH ENTER KEY!");
	CCamera::End();  //2D描画終了
}

//ゲーム画面
void CUi::Game()
{
	CCamera::Start(0, 800, 0, 600);
	//2D描画開始
	//描画色の設定(緑色の半透明)
	glColor4f(0.0f, 1.0f, 0.0f, 0.5f);
	//文字列編集エリアの作成
	char num[16];
	char hp[8];
	//文字列の設定
	sprintf(num, "NUM:%d", CEnemy::Instance()->Num());
	sprintf(hp, "HP:%d", CPlayer::Instance()->HP());
	//文字の描画
	mFont.Draw(700, 550, 10, 20, num);
	mFont.Draw(700, 500, 10, 20, hp);
	CCamera::End();  //2D描画終了
}

//ゲームオーバー表示
void CUi::Over()
{
	CCamera::Start(0, 800, 0, 600); //2D描画開始
	mFont.Draw(250, 300, 18, 36, "GAME OVER!!");
	CCamera::End();  //2D描画終了
}

//ゲームクリア表示
void CUi::Clear()
{
	CCamera::Start(0, 800, 0, 600);
	mFont.Draw(250, 300, 18, 36, "GAME CLEAR!!");
	CCamera::End();
}

//描画処理
void CUi::Render()
{
	switch (Scene())
	{
	case 1:	//タイトルシーン
		Title();
		break;
	case 2:	//ゲームシーン
		Game();
		break;
	case 3:	//ゲームオーバーシーン
		Over();
		break;
	case 4:	//ゲームクリアシーン
		Clear();
		break;
	default://どのシーンにも所属しない
		break;
	}
}

//経過時間の取得
void CUi::Time(int time)
{
	mTime = time;
}