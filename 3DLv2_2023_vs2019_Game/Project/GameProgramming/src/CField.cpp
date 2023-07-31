#include "CField.h"
#include "CTaskManager.h"

//コンストラクタ
CField::CField()
{
	//ポーズの種類を設定
	SetPauseType(TaskPauseType::eGame);
	//タスクの優先度設定
	mPriority = (int)TaskPriority::eBackGround;
	//タスクマネージャに自信を追加
	CTaskManager::Instance()->Add(this);

	mModel = new CModel();
	//モデルの入力
	mModel->Load("res\\Graund.obj", "res\\Graund.mtl");
	//読み込んだ背景モデルデータでコライダを生成
	mColliderMesh.Set(nullptr, nullptr, mModel);
}

//デストラクタ
CField::~CField()
{
	//タスクマネージャから自信を取り除く
	CTaskManager::Instance()->Remove(this);

	//背景モデルデータを破棄
	delete mModel;
	mModel = nullptr;
}

//更新処理
void CField::Update()
{
}

//描画処理
void CField::Render()
{
	if (mModel == nullptr) return;
	//背景モデルを描画
	mModel->Render();
}