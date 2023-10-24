#pragma once
#include "CModel.h"
#include "CObjectBase.h"
#include "CPlayer.h"

class CSword :public CObjectBase
{
public:
	// コンストラクタ
	CSword(const CVector& pos, const CVector& scale);
	// デストラクタ
	~CSword();

	// 更新処理
	void Update();
	// 描画処理
	void Render();

private:
	CModel* mpModel;
	CPlayer* mpPlayer;
};