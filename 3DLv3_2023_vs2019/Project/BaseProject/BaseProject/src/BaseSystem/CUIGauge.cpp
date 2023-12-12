#include "CUIGauge.h"
#include "CImage.h"
#include "Maths.h"

// コンストラクタ
CUIGauge::CUIGauge()
	: mMaxValue(0)
	, mValue(0)
	, mValueF(0)
{
}

// デストラクタ
CUIGauge::~CUIGauge()
{
}

// 最大値を設定
void CUIGauge::SetMaxValue(int value)
{
	mMaxValue = value;
}

// 現在値を設定
void CUIGauge::SetValue(int value)
{
	mValue = value;
}

// 現在値の設定(float型)
void CUIGauge::SetValueF(float valuef)
{
	mValueF = valuef;
}

// 更新
void CUIGauge::Update()
{
}