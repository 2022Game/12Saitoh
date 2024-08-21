#include "CNormalSwordEffect.h"

// コンストラクタ
CNormalSwordEffect::CNormalSwordEffect(const CVector& start, const CVector& end)
	: CLineEffect(ETag::eEffect)
{

	CVector2 size = CVector2(64.0f, 256.0f);
	// テクスチャを設定
	SetTexture("NormalSwordEffect");
	// ベースUVを設定
	SetBaseUV(CRect(0.0f, 0.0f, size.X(), size.Y()));
	// エフェクトのアニメーションデータを生成して設定
	SetAnimData(new TexAnimData(1, 5, false, 5, 0.05f));

	// 線分エフェクトの始点と終点を設定
	float length = (start - end).Length();
	float width = length * (size.X() / size.Y());
	AddPoint(start, width, width);
	AddPoint(end, width, width);
}

// デストラクタ
CNormalSwordEffect::~CNormalSwordEffect()
{
	SAFE_DELETE(mpAnimData);
}

// 更新処理
void CNormalSwordEffect::Update()
{
	CLineEffect::Update();

	// エフェクトのアニメーションが終わったら、エフェクトを削除
	if (IsEndAnim())
	{
		Kill();
	}
}

// 描画処理
void CNormalSwordEffect::Render()
{
	//glDisable(GL_DEPTH_TEST);
	CLineEffect::Render();
	//glEnable(GL_DEPTH_TEST);
}