#pragma once
#include "CObjectBase.h"

class CModel;

class CTitleField : public CObjectBase
{
public:
	// コンストラクタ
	CTitleField();
	// ですとラクタ
	~CTitleField();

	// 更新処理
	void Update();
	// 描画処理
	void Render();

private:
	CModel* mpModel;
};