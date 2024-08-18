#include "CResultUIBase.h"
#include "CLightingButton.h"
#include "CText.h"

// �R���X�g���N�^
CResultUIBase::CResultUIBase()
	: CTask(ETaskPriority::eUI, 0, ETaskPauseType::eDefault,false, false)
	, mIsEnd(false)
	, mSelectIndex(0)
{
	// ���U���g���S�̃t�H���g�f�[�^�𐶐�
	mpResultFont = new CFont("res\\Font\\GoodChoice.ttf");
	mpResultFont->SetFontSize(120);
	mpResultFont->SetAlignment(FTGL::TextAlignment::ALIGN_CENTER);
	mpResultFont->SetLineLength(WINDOW_WIDTH);

	SetEnable(false);
	SetShow(false);
}

// �f�X�g���N�^
CResultUIBase::~CResultUIBase()
{
	SAFE_DELETE(mpResultFont);
	SAFE_DELETE(mpResultLogo);

	int size = mButton.size();
	for (int i = 0; i < size; i++)
	{
		CButton* btn = mButton[i];
		mButton[i] = nullptr;
		SAFE_DELETE(btn);
	}
	mButton.clear();
}

// �X�V����
void CResultUIBase::Update()
{
}

// �`�揈��
void CResultUIBase::Render()
{
}

// ���U���g��ʏI����
bool CResultUIBase::IsEnd() const
{
	return mIsEnd;
}

// �Q�[���ɍĒ��킷�邩
bool CResultUIBase::IsReTry() const
{
	// �I�����ڂ�1�ڂȂ�΁A�Ē���
	return mSelectIndex == 0;
}

// �^�C�g����ʂɖ߂邩
bool CResultUIBase::IsTitle() const
{
	// �I�����ڂ�2�ڂȂ�΁A�^�C�g����ʂɖ߂�
	return mSelectIndex == 1;
}

// [���g���C]�N���b�N���̃R�[���o�b�N�֐�
void CResultUIBase::OnClickReTry()
{
	if (mIsEnd) return;
	mSelectIndex = 0;
	mIsEnd = true;
}

// [�^�C�g���֖߂�]�N���b�N���̃R�[���o�b�N�֐�
void CResultUIBase::OnClickTitle()
{
	if (mIsEnd) return;
	mSelectIndex = 1;
	mIsEnd = true;
}

// UI�����J�n
void CResultUIBase::Start()
{
}

// UI�����I��
void CResultUIBase::End()
{
}