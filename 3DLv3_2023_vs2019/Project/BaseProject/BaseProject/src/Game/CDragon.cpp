#include "CDragon.h"
#include "Maths.h"

CDragon* CDragon::spInstance = nullptr;

// �G�̃A�j���[�V�����f�[�^�e�[�u��
const CDragon::AnimData CDragon::ANIM_DATA[] =
{
	{ "Character\\Dragon\\anim\\idle1.x",			true,	102.0f,		0.0f },	// �A�C�h��1
	{ "Character\\Dragon\\anim\\idle2.x",			true,	151.0f,		0.0f },	// �A�C�h��2
	{ "Character\\Dragon\\anim\\land.x",			false,	200.0f,		0.0f },	// ���n
	{ "Character\\Dragon\\anim\\run.x",				true,	40.0f,		0.0f },	// ����
	{ "Character\\Dragon\\anim\\walk.x",			true,	80.0f,		0.0f },	// ����
	{ "Character\\Dragon\\anim\\scream.x",			true,	202.0f,		0.0f },	// ���K
	{ "Character\\Dragon\\anim\\sleep.x",			true,	162.0f,		0.0f },	// �Q��
	{ "Character\\Dragon\\anim\\takeOff.x",			false,	121.0f,		0.0f },	// ����
	{ "Character\\Dragon\\anim\\attackflame.x",		false,	241.0f,		0.0f },	// �u���X�U��
	{ "Character\\Dragon\\anim\\attackHand.x",		false,	182.0f,		0.0f },	// ��ъ|����U��
	{ "Character\\Dragon\\anim\\attackMouth.x",		false,	86.0f,		0.0f },	// ���݂��U��
	{ "Character\\Dragon\\anim\\die.x",				false,	260.0f,		0.0f },	// ���S
	{ "Character\\Dragon\\anim\\flyFlame.x",		false,	182.0f,		0.0f },	// �󒆃u���X�U��
	{ "Character\\Dragon\\anim\\flyforward.x",		true,	80.0f,		0.0f },	// �󒆑O�i
	{ "Character\\Dragon\\anim\\flyGlide.x",		true,	102.0f,		0.0f },	// �󒆊���
	{ "Character\\Dragon\\anim\\flyIdle.x",			true,	102.0f,		0.0f },	// �󒆃A�C�h��
	{ "Character\\Dragon\\anim\\getHit.x",			true,	86.0f,		0.0f },	// �̂�����
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

	// �����_���l�𐶐����Ď擾�����l�ɂ��X�e�[�^�X��ݒ�
	int rand = Math::Rand(0.0f, 4.0f);
	mStatus = ENEMY_STATUS[rand];

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