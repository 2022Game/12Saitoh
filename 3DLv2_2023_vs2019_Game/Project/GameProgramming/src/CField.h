#pragma once
#include "CTask.h"
#include "CModel.h"
#include "CColliderMesh.h"

//フィールド用クラス
class CField : public CTask
{
public:
	//コンストラクタ
	CField();
	//デストラクタ
	~CField();

	//更新処理
	void Update();
	//描画処理
	void Render();

private:
	CModel* mModel;	//フィールドのモデルデータ
	CColliderMesh mColliderMesh;	//フィールドのコライダ
};