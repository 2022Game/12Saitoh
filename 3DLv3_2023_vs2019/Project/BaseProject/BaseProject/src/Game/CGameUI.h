#pragma once
#include"CUIBase.h"
#define TARGET_IMAGE "UI\\TargetUI.png"
#define SPMOVE_IMAGE "UI\\spmoveUI.png"
#define COUNTER_IMAGE "UI\\counterUI.png"
#define QKEY_IMAGE "UI\\Qkey.png"
#define CKEY_IMAGE "UI\\CKey.png"
#define LBUTTON_IMAGE "UI\\LButton.png"
#define RBUTTON_IMAGE "UI\\RButton.png"

class CImage;
// �Q�[�����ɕ\������UI�N���X
class CGameUI : public CUIBase
{
public:
	// �R���X�g���N�^
	CGameUI();
	// �f�X�g���N�^
	~CGameUI();

	// �X�V����
	void Update() override;
private:
	CImage* mpTargetUI;
	CImage* mpQKeyUI;
	CImage* mpCKeyUI1;
	CImage* mpCKeyUI2;
	CImage* mpSPMoveUI;
	CImage* mpCounterUI;
	CImage* mpLButtonUI;
	CImage* mpRButtonUI;
};