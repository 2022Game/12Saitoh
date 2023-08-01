#include "CGame.h"
#include "CApplication.h"
#include "CCamera.h"
#include "CTaskManager.h"
#include "main.h"

int CGame::sItemNum = 0;
int CGame::sNum = 0;

//コンストラクタ
CGame::CGame()
	: mpPlayer(nullptr)
	, mpSlime(nullptr)
	, mpWolf(nullptr)
	, mpBoss(nullptr)
	, mpBlock(nullptr)
	, mpBackGround(nullptr)
	, mpBackGround2(nullptr)
	, mpBackGround3(nullptr)
	, mpBackGround4(nullptr)
	, mpHeart(nullptr)
	, mInput()
	, mpUi(nullptr)    
	, mTime(0)
	, mCdx(0)
	, mCdy(0)
{
	mpUi = new CUi();
}

//デストラクタ
//デストラクタ
CGame::~CGame()
{
	//全てのインスタンス削除
	CTaskManager::Instance()->AllDelete();
	if (mpUi != nullptr)
	{	//UIを削除し、初期化
		delete mpUi;
		mpUi = 0;
	}
}

//ゲームクリア処理
void CGame::Clear()
{
	CTaskManager::Instance()->Update();
	CTaskManager::Instance()->Delete();
	CTaskManager::Instance()->Render();
	if (mpBackGround4 == nullptr)
	mpBackGround4 = new CBackGround4(640.0f, 400.0f, 640.0f, 400.0f, 0, 1281, 801, 0, CApplication::Texture5());
	mpUi->DrawResult();
}

//ゲームオーバー処理
void CGame::Over()
{
	CTaskManager::Instance()->Update();
	CTaskManager::Instance()->Delete();
	CTaskManager::Instance()->Render();
	if (mpBackGround4 == nullptr)
	mpBackGround4 = new CBackGround4(640.0f, 400.0f, 640.0f, 400.0f, 0, 1281, 801, 0, CApplication::Texture4());

}

//スタート処理
void CGame::Start()
{
	CTaskManager::Instance()->Update();
	CTaskManager::Instance()-> Render();
	if (mpBackGround4 == nullptr)
	mpBackGround4 = new CBackGround4(640.0f, 400.0f, 640.0f, 400.0f, 0, 1281, 801, 0, CApplication::Texture3());
}

void CGame::Stage1()
{
	while (sNum != 0) {
		sNum--;
	}
	delete mpBackGround4;
	mpBackGround4 = nullptr;

	mpBlock = new CBlock(640.0f, 500.0f, 1280.0f, 5.0f, CApplication::Texture9());
	mpBlock = new CBlock(640.0f, -20.0f, 1280.0f, 5.0f, CApplication::Texture9());
	mpBlock = new CBlock(-50.0f, 0.0f, 5.0f, 50.0f, CApplication::Texture9());
	mpBlock = new CBlock(1360.0f, 0.0f, 5.0f, 50.0f, CApplication::Texture9());
	mpPlayer = new CPlayer(400.0f, 250.0f, 110.0f, 110.0f, CApplication::Texture());
	mpSlime = new CSlime(1000.0f, 100.0f, 80.0f, 60.0f, CSlime::Texture6());
	mpSlime = new CSlime(800.0f, 300.0f, 80.0f, 60.0f, CSlime::Texture6());
	mpBackGround = new CBackGround(640.0f, 400.0f, 640.0f, 400.0f, 0, 1279, 799, 0, CApplication::Texture6());
	mpBackGround2 = new CBackGround2(640.0f, 400.0f, 640.0f, 400.0f, 0, 1279, 799, 0, CApplication::Texture7());
	mpBackGround3 = new CBackGround3(640.0f, 400.0f, 640.0f, 400.0f, 0, 1279, 799, 0, CApplication::Texture8());
	mpBackGround = new CBackGround(1920.0f, 400.0f, 640.0f, 400.0f, 0, 1279, 799, 0, CApplication::Texture6());
	mpBackGround2 = new CBackGround2(1920.0f, 400.0f, 640.0f, 400.0f, 0, 1279, 799, 0, CApplication::Texture7());
	mpBackGround3 = new CBackGround3(1920.0f, 400.0f, 640.0f, 400.0f, 0, 1279, 799, 0, CApplication::Texture8());
	mpBackGround = new CBackGround(3200.0f, 400.0f, 640.0f, 400.0f, 0, 1279, 799, 0, CApplication::Texture6());
	mpBackGround2 = new CBackGround2(3200.0f, 400.0f, 640.0f, 400.0f, 0, 1279, 799, 0, CApplication::Texture7());
	mpBackGround3 = new CBackGround3(3200.0f, 400.0f, 640.0f, 400.0f, 0, 1279, 799, 0, CApplication::Texture8());
}

void CGame::Stage2()
{
	mpWolf = new CWolf(1000.0f, 200.0f, 80.0f, 60.0f, CWolf::Texture7());
	mpWolf = new CWolf(900.0f, 300.0f, 80.0f, 60.0f, CWolf::Texture7());
	mpWolf = new CWolf(800.0f, 100.0f, 80.0f, 60.0f, CWolf::Texture7());
}

void CGame::Boss()
{
	//ボスの初期化
	if (mpBoss != nullptr)
	{
		delete mpBoss;
		mpBoss = nullptr;
	}
	mpBoss = new CBoss(1000.0f, 300.0f, 240.0f, 240.0f, CBoss::Texture8());
}

void CGame::SetItem()
{
	mpItem = new CItem(400.0f, 200.0f, 30.0f, 30.0f, CApplication::Texture9());
}

//更新処理
void CGame::Update()
{
	sNum = CWolf::Num() + CSlime::Num() + CBoss::Num();
	sItemNum = CItem::Num();
	//更新、衝突、描画
	CTaskManager::Instance()->Update();
    CTaskManager::Instance()->Collision();
	CTaskManager::Instance()->Delete();
	CTaskManager::Instance()->Render();
	
	//UI
	mpUi->Time(mTime++);
	mpUi->Hp(CPlayer::HP());
	mpUi->Stamina(CPlayer::Stamina());
	mpUi->Render();
	if (mpBoss != nullptr)
		mpUi->BossRender();
}

int CGame::Num()
{
	return sNum;
}

int CGame::ItemuNum()
{
	return sItemNum;
}