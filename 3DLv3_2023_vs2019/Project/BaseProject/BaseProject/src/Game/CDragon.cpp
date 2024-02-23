#include "CDragon.h"
#include "Maths.h"
#include "CPlayer.h"
#include "Primitive.h"
#include "CColliderSphere.h"
#include "CColliderLine.h"
#include "CDebugPrint.h"

#define GRAVITY			0.0625f	// �d��
#define ENEMY_HEIGHT	400.0f
#define FOV_LANGE		170.0f
#define FOV_ANGLE		60.0f

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
	, mMoveSpeed(CVector::zero)
	, mIsGrounded(true)
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

	// �ڒn����p�̐����R���C�_�̐ݒ�
	mpColliderLine = new CColliderLine
	(
		this, ELayer::eField,
		CVector(0.0f, 0.0f, 0.0f),
		CVector(0.0f, ENEMY_HEIGHT, 0.0f)
	);

	// �R���C�_�[�̐���
	mpColliderSphere = new CColliderSphere(this, ELayer::eEnemy, 200.0f);
	mpColliderSphere->SetCollisionLayers({ ELayer::ePlayer });
	mpColliderSphere->SetCollisionTags({ ETag::ePlayer });

	mpDamageCol = new CColliderSphere(this, ELayer::eDamageCol, 500.0f);
	mpDamageCol->SetCollisionLayers({ ELayer::eAttackCol });
	//mpDamageCol->SetCollisionTags({ ETag::eSowrd });
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

// �v���C���[�����������ǂ���
bool CDragon::IsFoundPlayer() const
{
	CVector playerPos = CPlayer::Instance()->Position();
	CVector enemyPos = Position();

	// ����p�x�̔���
	// ���g����v���C���[�܂ł̃x�N�g�����擾
	CVector EP = (playerPos - enemyPos).Normalized();
	// ���g�̐��ʕ����̃x�N�g�����擾
	CVector forward = VectorZ().Normalized();
	// ���ʕ����̃x�N�g���ƃv���C���[�܂ł�
	// ���ς���p�x�����߂�
	float dotZ = forward.Dot(EP);

	// ���߂��p�x������p�x�O�̏ꍇ�Afalse��Ԃ�
	if (dotZ <= cosf(FOV_ANGLE * M_PI / 180.0f)) return false;

	// �����̔���
	// ���g����v���C���[�܂ł̋��������߂�
	float distance = (playerPos - enemyPos).Length();
	// ���߂����������싗�����������ꍇ�Afalse��Ԃ�
	if (distance > FOV_LANGE) return false;

	// ���씻��Ƌ��������ʂ����̂�true��Ԃ�
	return true;
}

// �X�V����
void CDragon::Update()
{
	// ��Ԃɍ��킹�čX�V������؂�ւ���
	switch (mState)
	{
		// �ҋ@���
	case EState::eIdle:
		UpdateIdle();
		break;
		// �ړ����
	case EState::eMove:
		break;
		// �U�����
	case EState::eAttack:
		break;
		// ���S���
	case EState::eDeath:
		break;
	}

	// �d�͂̐ݒ�
	mMoveSpeed -= CVector(0.0f, GRAVITY, 0.0f);

	// �ړ�
	Position(Position() + mMoveSpeed);

	// �ړ������֌�����
	CVector current = VectorZ();
	CVector target = mMoveSpeed;
	target.Y(0.0f);
	target.Normalize();
	CVector forward = CVector::Slerp(current, target, 0.0f);
	Rotation(CQuaternion::LookRotation(forward));

	CXCharacter::Update();

#ifdef _DEBUG
	// �h���S���̃X�e�[�^�X��\��
	CDebugPrint::Print("�G�X�e�[�^�X\n");
	CDebugPrint::Print("���x�� : %d\nHP : %d\n�U���� : %d\n�h��� : %d",
		mStatus.level, mStatus.hp, mStatus.atk, mStatus.def);
#endif
}

// �Փˏ���
void CDragon::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	// �n�ʂƂ̓����蔻��
	if (self == mpColliderLine)
	{
		if (other->Layer() == ELayer::eField)
		{
			mMoveSpeed.Y(0.0f);
			// �߂荞�܂Ȃ��悤�ɒ���
			Position(Position() + hit.adjust);
			mIsGrounded = true;
		}
	}
}

// �_���[�W����
void CDragon::TakeDamage(int damage)
{
	mStatus.hp -= damage;
	if (mStatus.hp <= 0)
	{
		// ���S����
	}
	else
	{
		// �̂�����Ȃǂ̔�e����
	}
}

// �`�揈��
void CDragon::Render()
{
	CXCharacter::Render();

	Primitive::DrawSector(
		Position() + CVector(0.0f, 1.0, 0.0f),
		-EulerAngles(),
		-FOV_ANGLE,
		FOV_ANGLE,
		FOV_LANGE,
		CColor::red,
		45
	);
}

// �C���X�^���X���擾
CDragon* CDragon::Instance()
{
	return spInstance;
}