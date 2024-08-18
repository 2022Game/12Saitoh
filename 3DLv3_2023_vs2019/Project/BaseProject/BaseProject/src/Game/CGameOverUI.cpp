#include "CGameOverUI.h"
#include "CText.h"
#include "CTaskManager.h"
#include "CLightingButton.h"

// �R���X�g���N�^
CGameOverUI::CGameOverUI()
{
	// ���U���g���S�𐶐�
	mpResultLogo = new CText
	(
		mpResultFont, 128,
		CVector2(0.0f, 32.0f),
		CVector2(WINDOW_WIDTH, WINDOW_HEIGHT),
		CColor(1.01f, 0.1f, 0.1f),
		ETaskPriority::eUI,
		0,
		ETaskPauseType::eDefault,
		false,
		false
	);
	mpResultLogo->SetText("GameOver");
	mpResultLogo->SetEnableOutline(true);
	mpResultLogo->SetOutlineColor(CColor(0.9f, 0.9f, 0.9f));

	// [�^�C�g���֖߂�]�{�^���𐶐�
	CLightingButton* btn1 = new CLightingButton
	(
		CVector2(400.0f, 320.0f),
		CVector2(181.0f * 1.5f, 47.0f * 1.5f),
		ETaskPriority::eUI, 0, ETaskPauseType::eSystem,
		false, false
	);
	// �{�^���̉摜��ǂݍ���
	btn1->LoadButtonImage("UI/result_title.png", "UI/result_title.png");
	// �{�^���N���b�N���ɌĂяo�����R�[���o�b�N�֐���ݒ�
	btn1->SetOnClickFunc(std::bind(&CResultUIBase::OnClickTitle, this));
	btn1->SetEnable(false);
	btn1->SetScale(1.0f);
	// �{�^�����X�g�ɒǉ�
	mButton.push_back(btn1);

	SetEnable(false);
	SetShow(false);
}

// �f�X�g���N�^
CGameOverUI::~CGameOverUI()
{
	// �|�[�Y������
	CTaskManager::Instance()->UnPause(PAUSE_MENU_OPEN);
}

// �X�V����
void CGameOverUI::Update()
{
	mpResultLogo->Update();
	for (CButton* btn : mButton)
	{
		btn->Update();
	}
}

// �`�揈��
void CGameOverUI::Render()
{
	// ���U���g���S�`��
	mpResultLogo->Render();

	for (CButton* btn : mButton)
	{
		btn->Render();
	}
}

// UI�����J�n
void CGameOverUI::Start()
{
	for (CButton* btn : mButton)
	{
		btn->SetEnable(true);
	}

	SetEnable(true);
	SetShow(true);
	CTaskManager::Instance()->Pause(PAUSE_MENU_OPEN);
}

// UI�����I��
void CGameOverUI::End()
{
	for (CButton* btn : mButton)
	{
		btn->SetEnable(false);
	}
	SetEnable(false);
	SetShow(false);
	// �|�[�Y������
	CTaskManager::Instance()->UnPause(PAUSE_MENU_OPEN);
}
