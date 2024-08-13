#pragma once
#include "CXCharacter.h"
#include "CModel.h"

class CTitleSword;
class CTitleModel : public CXCharacter
{
public:
	// �C���X�^���X�̃|�C���^�̎擾
	static CTitleModel* Instance();

	// �R���X�g���N�^
	CTitleModel();
	// �f�X�g���N�^
	~CTitleModel();

	// �X�V����
	void Update();
	// �`�揈��
	void Render();

	// ���������ǂ���
	bool IsDrawn();
private:
	// �A�j���[�V�����f�[�^
	struct AnimData
	{
		std::string path;	// �A�j���[�V�����f�[�^�̃p�X
		bool loop;			// ���[�v���邩�ǂ���
		float frameLength;	// �A�j���[�V�����̃t���[����
	};
	// �A�j���[�V�����f�[�^�̃e�[�u��
	static const AnimData ANIM_DATA[];

	// �A�j���[�V�����̎��
	enum class EAnimType 
	{
		eNone = -1,
		
		eTPose,				// T�|�[�Y
		eIdle,				// �ҋ@
		eIdle_Combat,		// �ҋ@(����)
		eIdle_Drawn_Combat,	// �ҋ@����������

		Num
	};
	void ChangeAnimation(EAnimType type);

	// �C���X�^���X
	static CTitleModel* spInstance;

	// �^�C�g�����f���p�̌�
	CTitleSword* mpTitleSword;
	// �X�V�����̒i�K
	int mUpdateStep;
	// ���������ǂ���
	bool mIsDrawn;
};