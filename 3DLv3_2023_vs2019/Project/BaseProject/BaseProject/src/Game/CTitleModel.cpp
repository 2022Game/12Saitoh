#include "CTitleModel.h"
#include "CInput.h"
#include "CTitleSword.h"

#define DRAWN_FRAME 35.0f

// �C���X�^���X
CTitleModel* CTitleModel::spInstance = nullptr;

// �^�C�g���p���f���̃A�j���[�V�����f�[�^�̃e�[�u��
const CTitleModel::AnimData CTitleModel::ANIM_DATA[] =
{
	/*				�@�p�X								�@ ���[�v  �t���[��	*/
	{ "",													true,	 0.0f,	},	// T�|�[�Y
	{ "Character\\Player\\anim\\idle.x",					true,	 601.0f,},	// �ҋ@(�[��)
	{ "Character\\Player\\anim\\idle_combat.x",				true,	 601.0f,},	// �ҋ@(����)
	{ "Character\\Player\\anim\\idle_drawn_combat.x",		false,	 101.0f,},	// �ҋ@����������
};

// �R���X�g���N�^
CTitleModel::CTitleModel()
	: CXCharacter(ETag::ePlayer, ETaskPriority::ePlayer,
		0, ETaskPauseType::ePlayer)
	, mUpdateStep(0)
	, mIsDrawn(false)
{
	// �C���X�^���X��ݒ�
	spInstance = this;

	// ���f���f�[�^�擾
	CModelX* model = CResourceManager::Get<CModelX>("TitleModel");

	// �e�[�u�����̃A�j���[�V�����f�[�^��ǂݍ���
	int size = ARRAY_SIZE(ANIM_DATA);
	for (int i = 0; i < size; i++)
	{
		const AnimData& data = ANIM_DATA[i];
		if (data.path.empty()) continue;
		model->AddAnimationSet(data.path.c_str());
	}
	// CXCharacter�̏�����
	Init(model);

	// �ŏ��͑ҋ@�A�j���[�V�������Đ�
	ChangeAnimation(EAnimType::eIdle);
	SetAnimationSpeed(0.5f);

	// �^�C�g�����f���p�̌��𐶐�
	mpTitleSword = new CTitleSword();
	mpTitleSword->Rotate(CVector(0.0f, 0.0f, 0.0f));
}

// �f�X�g���N�^
CTitleModel::~CTitleModel()
{
}

// �C���X�^���X���擾
CTitleModel* CTitleModel::Instance()
{
	return spInstance;
}

// �A�j���[�V������؂�ւ�
void CTitleModel::ChangeAnimation(EAnimType type)
{
	if (!(EAnimType::eNone < type && type < EAnimType::Num)) return;
	AnimData data = ANIM_DATA[(int)type];
	CXCharacter::ChangeAnimation((int)type, data.loop, data.frameLength, 0.0f);
}

// �X�V����
void CTitleModel::Update()
{
	switch (mUpdateStep)
	{
	case 0:// �ҋ@(�[��)���̏���
		// �}�E�X�N���b�N���ɔ���������s��
		if (CInput::PushKey(VK_LBUTTON))
		{
			ChangeAnimation(EAnimType::eIdle_Drawn_Combat);
			SetAnimationSpeed(0.7f);
			mUpdateStep++;
		}
		break;
	case 1:// �������̏���
		// �������̌��̎����ւ�����
		if (DRAWN_FRAME <= GetAnimationFrame())
		{
			mIsDrawn = true;
			mUpdateStep++;
		}
		break;
	case 2:
		// �������삪�I�������ҋ@(����)�ɐ؂�ւ���
		if (IsAnimationFinished())
		{
			ChangeAnimation(EAnimType::eIdle_Combat);
			SetAnimationSpeed(0.5f);
			mUpdateStep++;
		}
		break;
	case 3:// �ҋ@(����)���̏���
		break;
	}
	// �L�����N�^�[�̍X�V
	CXCharacter::Update();
}

// �`�揈��
void CTitleModel::Render()
{
	CXCharacter::Render();
}

// ���������ǂ���
bool CTitleModel::IsDrawn()
{
	return mIsDrawn;
}