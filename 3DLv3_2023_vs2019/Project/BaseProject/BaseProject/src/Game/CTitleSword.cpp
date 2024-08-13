#include "CTitleSword.h"
#include "CTitleModel.h"

// コンストラクタ
CTitleSword::CTitleSword()
{
	mpModel = CResourceManager::Get<CModel>("Sword");
}

// デストラクタ
CTitleSword::~CTitleSword()
{
}

// 更新処理
void CTitleSword::Update()
{
	// 納刀状態と抜刀状態で親のボーンを変更疑問
	if (CTitleModel::Instance()->IsDrawn())
	{
		SetAttachMtx(CTitleModel::Instance()->GetFrameMtx("Armature_weapon_r"));
	}
	else
	{
		SetAttachMtx(CTitleModel::Instance()->GetFrameMtx("Armature_sword_holder"));
	}
}

// 描画処理
void CTitleSword::Render()
{
	mpModel->Render(Matrix());
}

// 武器の行列取得
CMatrix CTitleSword::Matrix() const
{
	// 手に持っていない時は、自分自身の行列を返す
	if (mpAttachMtx == nullptr)
	{
		return CTransform::Matrix();
	}
	// 手に持っている時は、アタッチしている行列を返す
	else
	{
		CMatrix sm;
		CMatrix rm;
		sm.Scale(12.0f, 12.0f, 12.0f);
		// 納刀状態と抜刀状態か判定し、描画の調整を行う
		if (CTitleModel::Instance()->IsDrawn()) //抜刀
		{
			rm = rm.RotateY(-90.0f);
			return rm * sm * (*mpAttachMtx);
		}
		else //納刀
		{
			rm.RotateX(-90.0f);
			return rm * sm * (*mpAttachMtx);
		}
	}
}