#include "CTask.h"
#include "CTaskManager.h"
#include "CSceneManager.h"

//コンストラクタ
CTask::CTask(ETaskPriority prio, int sortOrder,
	ETaskPauseType pause, bool dontDelete, bool addTaskList)
	: mPriority(prio)
	, mSortOrder(sortOrder)
	, mEnabled(true)
	, mPauseType(pause)
	, mAddTaskList(addTaskList)
{
	//シーン遷移で破棄しないタスクかどうかで
	//所属するシーンのタイプを切り替える
	mSceneType = dontDelete
		? EScene::eNone
		: CSceneManager::Instance()->GetCurrentScene();

	// 自動登録がオンならば
	if (mAddTaskList)
	{
		//タスクを生成した時に
		//タスクマネージャーのタスクリストに追加
		CTaskManager::Instance()->Add(this);
	}
}

//デストラクタ
CTask::~CTask()
{
	if (mAddTaskList)
	{
		//タスクマネージャーのタスクリストから取り除く
		CTaskManager::Instance()->Remove(this);
	}
}

//更新
void CTask::Update()
{
}

//描画
void CTask::Render()
{
}

//優先度を設定
void CTask::SetPriority(ETaskPriority prio)
{
	//既に設定されている優先度であれば処理しない
	if (mPriority == prio) return;

	//一度タスクリストから取り除き、再度追加することで、
	//新しい優先度の場所にタスクを差し込む
	mPriority = prio;
	CTaskManager::Instance()->Remove(this);
	CTaskManager::Instance()->Add(this);
}

//優先度を取得
ETaskPriority CTask::GetPriority() const
{
	return mPriority;
}

//優先度内の順番を設定
void CTask::SetSortOrder(int sortOder)
{
	//既に設定されている順番であれば処理しない
	if (mSortOrder == sortOder) return;

	//一度タスクリストから取り除き、再度追加することで、
	//新しい優先度の場所にタスクを差し込む
	mSortOrder = sortOder;
	CTaskManager::Instance()->Remove(this);
	CTaskManager::Instance()->Add(this);
}

//優先度内の順番を取得
int CTask::GetSortOrder() const
{
	return mSortOrder;
}

//ポーズの種類を設定
void CTask::SetPauseType(ETaskPauseType type)
{
	mPauseType = type;
}

//ポーズの種類を取得
ETaskPauseType CTask::GetPauseType() const
{
	return mPauseType;
}

//所属するシーンを設定
void CTask::SetSceneType(EScene scene)
{
	mSceneType = scene;
}

//所属するシーンを取得
EScene CTask::GetSceneType() const
{
	return mSceneType;
}

//タスクを削除
void CTask::Kill()
{
	mEnabled = false;
}

//削除フラグ取得
bool CTask::IsKill() const
{
	return !mEnabled;
}