#ifndef CTASK_H
#define CTASK_H
#include "TaskPriority.h"
#include "TaskPauseType.h"
#include "SceneType.h"
class CTaskManager;
class CCollisionManager;

/*
タスククラス
タスクリストの要素
*/
class CTask {
	friend CTaskManager;
	friend CCollisionManager;
public:
	//衝突処理
	virtual void Collision() {}

	//デフォルトコンストラクタ
	CTask(EScene scene = EScene::eNone);
	//デストラクタ virtualにしないと子クラスのデストラクタが呼ばれない
	virtual ~CTask();
	//更新
	virtual void Update();
	//描画
	virtual void Render();

	//ポーズの種類を設定
	void SetPauseType(TaskPauseType type);
	//ポーズの種類を取得
	TaskPauseType GetPauseType() const;

	//非表示設定
	void SetHidden(bool isHidden);
	//非表示状態かどうか
	bool IsHidden() const;
	//所属するシーンを設定
	void SetSceneType(EScene scene);
	//所属するシーンを取得
	EScene GetSceneType()const;
protected:
	int mPriority;	//優先度
	bool mEnabled;	//有効フラグ
	bool mIsHidden;	//非表示フラグ
private:
	CTask* mpNext;	//次のポインタ
	CTask* mpPrev; //前のポインタ
	TaskPauseType mPauseType; //ポーズの種類
	EScene mSceneType;	//所属するシーンの種類
};

#endif
