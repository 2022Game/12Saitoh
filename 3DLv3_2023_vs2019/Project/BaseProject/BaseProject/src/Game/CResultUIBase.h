#pragma once
#include "CTask.h"

class CFont;
class CText;
class CLightingButton;
// ���U���g�V�[����UI�x�[�X�N���X
class CResultUIBase : public CTask
{
public:
	// �R���X�g���N�^
	CResultUIBase();
	// �f�X�g���N�^
	~CResultUIBase();

	// �X�V����
	virtual void Update();
	// �`�揈��
	virtual void Render();

	// ���U���g�I����ʂ�
	bool IsEnd() const;
	// �Q�[���ɍĒ��킷�邩
	bool IsReTry() const;
	// �^�C�g����ʂɖ߂邩
	bool IsTitle() const;

	// UI�����J�n
	virtual void Start();
	// UI�����I��
	virtual void End();
	// [���g���C]�N���b�N�̃R�[���o�b�N�֐�
	void OnClickReTry();
	// [�^�C�g���֖߂�]�N���b�N���̃R�[���o�b�N�֐�
	void OnClickTitle();

private:
	bool mIsEnd;		// ���U���g��ʏI���t���O
	int mSelectIndex;	// ���ݑI�����Ă��鍀��

protected:
	CFont* mpResultFont;	// ���U���g���S�̃t�H���g
	CText* mpResultLogo;	// ���U���g���S
	std::vector<CLightingButton*> mButton;
};