#include "CApplication.h"
//OpenGL
#include "glut.h"
#include "CTaskManager.h"
#include "CCollisionManager.h"
#include "CSceneManager.h"
#include "CTitleScene.h"
#include "CDebugProfiler.h"

CApplication::~CApplication()
{
}

void CApplication::Start()
{
	CSceneManager::Instance()->LoadScene(EScene::eTitle);
}

void CApplication::Update()
{
	//CDebugProfiler::StartTimer("Delete");
	CTaskManager::Instance()->Delete();
	//CDebugProfiler::EndTimer("Delete");

	//CDebugProfiler::StartTimer("Update");
	// タスクの更新処理
	CTaskManager::Instance()->Update();
	//CDebugProfiler::EndTimer("Update");

	//CDebugProfiler::StartTimer("Collision");
	// 衝突処理
	CCollisionManager::Instance()->CollisionAll();
	//CDebugProfiler::EndTimer("Collision");

	//CDebugProfiler::StartTimer("Scene");
	CSceneManager::Instance()->Update();
	//CDebugProfiler::EndTimer("Scene");

	//CDebugProfiler::StartTimer("Render");
	// タスクの描画処理
	CTaskManager::Instance()->Render();
	//CDebugProfiler::EndTimer("Render");

	//CDebugProfiler::StartTimer("ColRender");
	// コライダの描画
	CCollisionManager::Instance()->Render();
	//CDebugProfiler::EndTimer("ColRender");
}
