#pragma once
#include "CObjectBase.h"
#include "CWeapon.h"
#include "CTitleModel.h"

class CTitleSword : public CWeapon
{
public:
	// コンストラクタ
	CTitleSword();
	// デストラクタ
	~CTitleSword();

	// 更新処理
	void Update();
	// 描画処理
	void Render();

	// 行列の取得
	CMatrix Matrix() const override;
private:
	CModel* mpModel;
};