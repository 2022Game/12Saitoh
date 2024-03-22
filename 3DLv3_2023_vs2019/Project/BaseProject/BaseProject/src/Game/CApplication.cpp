#include "CApplication.h"
//OpenGL
#include "glut.h"
#include "CTaskManager.h"
#include "CCollisionManager.h"
#include "CSoundManager.h"
#include "CBGMManager.h"
#include "CSceneManager.h"
#include "CGamePause.h"
#include "CFade.h"

#include "CDebugProfiler.h"
CApplication::~CApplication()
{
}

void CApplication::Start()
{
#if _DEBUG
	// デバッグカメラを作成
	CDebugCamera::DebugCamera();
#endif

	// フェードクラスを作成
	CFade::Instance();
	// ゲームポーズクラスを作成
	new CGamePause();
	// サウンド管理クラスを作成
	CSoundManager::Instance();
	// BGM管理クラスを作成
	CBGMManager::Instance();

#if _DEBUG
	// デバッグモードでは、ブートメニューを最初に開く
	CSceneManager::Instance()->LoadScene(EScene::eBootMenu);
#else
	// リリースモードでは、タイトル画面を最初に開く
	CSceneManager::Instance()->LoadScene(EScene::eTitle);
#endif
}

void CApplication::End()
{
	CSceneManager::ClearInstance();
	CTaskManager::ClearInstance();
	CCollisionManager::ClearInstance();
	CResourceManager::ClearInstance();
	CBGMManager::ClearInstance();
	CSoundManager::ClearInstance();
}

void CApplication::Update()
{
	CTaskManager::Instance()->Delete();
	// タスクの更新処理
	CDebugProfiler::StartTimer("更新処理");
	CTaskManager::Instance()->Update();
	CDebugProfiler::EndTimer("更新処理");
	// 衝突処理
	CDebugProfiler::StartTimer("衝突処理");
	CCollisionManager::Instance()->CollisionAll();
	CDebugProfiler::EndTimer("衝突処理");

	// サウンドの更新
	CDebugProfiler::StartTimer("サウンド処理");
	CSoundManager::Instance()->Update();
	CDebugProfiler::EndTimer("サウンド処理");

	// シーンの更新
	CDebugProfiler::StartTimer("シーン処理");
	CSceneManager::Instance()->Update();
	CDebugProfiler::EndTimer("シーン処理");

	// タスクの描画処理
	CDebugProfiler::StartTimer("描画処理");
	CTaskManager::Instance()->Render();
	CDebugProfiler::EndTimer("描画処理");
	// コライダの描画
	CDebugProfiler::StartTimer("コライダ描画処理");
	CCollisionManager::Instance()->Render();
	CDebugProfiler::EndTimer("コライダ描画処理");
}