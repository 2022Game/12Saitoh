#include "CUIGauge.h"
#include "CImage.h"
#include "Maths.h"

// �R���X�g���N�^
CUIGauge::CUIGauge()
	: mMaxValue(0)
	, mValue(0)
	, mValueF(0)
{
}

// �f�X�g���N�^
CUIGauge::~CUIGauge()
{
}

// �ő�l��ݒ�
void CUIGauge::SetMaxValue(int value)
{
	mMaxValue = value;
}

// ���ݒl��ݒ�
void CUIGauge::SetValue(int value)
{
	mValue = value;
}

// ���ݒl�̐ݒ�(float�^)
void CUIGauge::SetValueF(float valuef)
{
	mValueF = valuef;
}

// �X�V
void CUIGauge::Update()
{
}