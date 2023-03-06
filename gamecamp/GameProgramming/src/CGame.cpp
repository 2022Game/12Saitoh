#include "CGame.h"
#include "CApplication.h"
#include "CCamera.h"
#include "main.h"

CGame::CGame()
	: mpUi(nullptr)
	, mTime(0)
	, mCdx(0)
	, mCdy(0)
{	
	//テクスチャの入力
	CApplication::Texture()->Load(TEXTURE);
	//背景テクスチャの入力
	CApplication::Texture2()->Load(BACKTEXTURE);
	mpUi = new CUi();
	//背景をキャラクタマネージャに追加する
	CApplication::CharacterManager()->Add(
		new CBack(640, 400, 1200, 400, CApplication::Texture2()));
	//プレイヤーをキャラクタマネージャに追加する
	CApplication::CharacterManager()->Add(
		new CPlayer(TIPSIZE * 10, TIPSIZE * 5, TIPSIZE, TIPSIZE, CApplication::Texture()));
	//敵をキャラクタマネージャに追加する
	CApplication::CharacterManager()->Add(
		mpEnemy = new CEnemy(TIPSIZE * 15, TIPSIZE * 4.5, TIPSIZE, TIPSIZE, CApplication::Texture()));
	CApplication::CharacterManager()->Add(
		mpEnemy2 = new CEnemy(TIPSIZE * 20, TIPSIZE * 7, TIPSIZE, TIPSIZE, CApplication::Texture()));
}

void CGame::Start()
{
	//ゲームの描画
	CApplication::CharacterManager()->Render();
	//UI処理
	mpUi->Render();
	mpUi->Start();
}

//bool CGame::IsOver()
//{
//
//}

void CGame::Over()
{
	CameraSet();
	//ゲームの描画
	CApplication::CharacterManager()->Render();
	CCamera::End();
	//UI処理
	mpUi->Render();
	mpUi->Over();
}

CGame::~CGame()
{
	//全てのインスタンス削除
	CApplication::CharacterManager()->AllDelete();
	//UIを生成している時
	if (mpUi != nullptr)
	{	//UIを削除し、初期化
		delete mpUi;
		mpUi = nullptr;
	}
}

//bool CGame::IsClear()
//{
//
//}

void CGame::Clear()
{
	CameraSet();
	//ゲームの描画
	CApplication::CharacterManager()->Render();
	CCamera::End();
	//UI処理
	mpUi->Render();
	mpUi->Clear();
}

void CGame::Update()
{
	//更新、衝突、削除、描画
	CApplication::CharacterManager()->Update();
	CApplication::CharacterManager()->Collision();
	CApplication::CharacterManager()->Delete();
	CameraSet();
	CApplication::CharacterManager()->Render();
	CCamera::End();
	//UI
	mpUi->Time(mTime++);
	mpUi->Render();
}

void CGame::CameraSet()
{

}
