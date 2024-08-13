#include "CTitleField.h"
#include "CModel.h"

// コンストラクタ
CTitleField::CTitleField()
	: CObjectBase(ETag::eField, ETaskPriority::eBackground)
{
	mpModel = CResourceManager::Get<CModel>("TitleField");
}

// デストラクタ
CTitleField::~CTitleField()
{
}

// 更新処理
void CTitleField::Update()
{
}

// 描画処理
void CTitleField::Render()
{
	mpModel->Render(Matrix());
}