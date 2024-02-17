#include "CDragon.h"

CDragon* CDragon::spInstance = nullptr;

// �G�̃A�j���[�V�����f�[�^�e�[�u��
const CDragon::AnimData CDragon::ANIM_DATA[] =
{
	{"Charcter\\Dragon\\anim\\Idle1.x",			true,	40.0f,	0.0f},	// �A�C�h��
};

// �R���X�g���N�^
CDragon::CDragon()
	: CXCharacter(ETag::eEnemy, ETaskPriority::eEnemy)
{
	// �C���X�^���X�̐ݒ�
	spInstance = this;

	// ���f���f�[�^�擾
	CModelX* model = CResourceManager::Get<CModelX>("Dragon");

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

	// �ŏ��͑ҋ@�̃A�j���[�V�������Đ�
	ChangeAnimation(EAnimType::eIdle1);

	// ��Ԃ̐ݒ�
	mState = EState::eIdle;
}

// �f�X�g���N�^
CDragon::~CDragon()
{

}

// �A�j���[�V�����̐؂�ւ�
void CDragon::ChangeAnimation(EAnimType type)
{
	if (!(EAnimType::None < type && type < EAnimType::Num)) return;
	AnimData data = ANIM_DATA[(int)type];
	CXCharacter::ChangeAnimation((int)type, data.loop, data.frameLength, data.motionValue);
}

// �X�V����
void CDragon::Update()
{
	CXCharacter::Update();
}

// �Փˏ���
void CDragon::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{

}

// �`�揈��
void CDragon::Render()
{
	CXCharacter::Render();
}

// �C���X�^���X���擾
CDragon* CDragon::Instance()
{
	return spInstance;
}