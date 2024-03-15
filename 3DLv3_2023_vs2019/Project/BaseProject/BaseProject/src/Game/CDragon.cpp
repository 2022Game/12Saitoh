#include "CDragon.h"
#include "Maths.h"
#include "CPlayer.h"
#include "Primitive.h"
#include "CColliderSphere.h"
#include "CColliderLine.h"
#include "CDebugPrint.h"

CDragon* CDragon::spInstance = nullptr;

// �R���X�g���N�^
CDragon::CDragon()
	: CXCharacter(ETag::eEnemy, ETaskPriority::eEnemy)
	, mMoveSpeed(CVector::zero)
	, mIsGrounded(true)
	, mIsAngry(false)
	, mAngryStandardValue(0)
	, mAngryValue(0)
	, mAngryElapsedTime(0.0f)
	, mElapsedTime(0.0f)
{
	// �C���X�^���X�̐ݒ�
	spInstance = this;

	// ���f���f�[�^�擾
	CModelX* model = CResourceManager::Get<CModelX>("Dragon");

	// �e�[�u�����̃A�j���[�V�����f�[�^��ǂݍ���
	int size = DragonData::GetAnimDataSize();
	for (int i = 0; i < size; i++)
	{
		const DragonData::AnimData& data = DragonData::GetAnimData(i);
		if (data.path.empty()) continue;
		model->AddAnimationSet(data.path.c_str());
	}
	// CXCharacter�̏�����
	Init(model);

	// �ŏ��͑ҋ@�̃A�j���[�V�������Đ�
	ChangeAnimation(EDragonAnimType::eIdle1);

	// �����_���l�𐶐����Ď擾�����l�ɂ��X�e�[�^�X��ݒ�
	int rand = Math::Rand(0.0f, 5.0f);
	mStatus = ENEMY_STATUS[rand];
	mMaxStatus = mStatus;

	// �{��l�̍ő�l��ݒ�
	// �ő�l�͍ő�HP�̂P������Ƃ��Đݒ�
	mAngryStandardValue = mMaxStatus.hp / 10;

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
	// �v���C���[�Ƃ̉����߂��p�R���C�_�[
	mpBodyCol = new CColliderSphere(this, ELayer::eEnemy, 200.0f);
	mpBodyCol->SetCollisionLayers({ ELayer::ePlayer });
	mpBodyCol->SetCollisionTags({ ETag::ePlayer });

	// �_���[�W���󂯂�p�̃R���C�_�[
	mpDamageCol = new CColliderSphere(this, ELayer::eDamageCol, 500.0f);
	mpDamageCol->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageCol->SetCollisionTags({ ETag::eWeapon });

	// �U���p�̃R���C�_�[
	mpAttackMouthCol = new CColliderSphere(this, ELayer::eAttackCol, 1.5f);
	mpAttackMouthCol->SetCollisionLayers({ ELayer::eDamageCol });
	mpAttackMouthCol->SetCollisionTags({ ETag::ePlayer });
	// �R���C�_�[���A�^�b�`
	const CMatrix* attackcol = GetFrameMtx("Armature_Tongue02");
	mpAttackMouthCol->SetAttachMtx(attackcol);

	// �ŏ��͍U������p�̃R���C�_�[�̓I�t�ɂ��Ă���
	mpAttackMouthCol->SetEnable(false);
}

// �f�X�g���N�^
CDragon::~CDragon()
{

}

// �C���X�^���X���擾
CDragon* CDragon::Instance()
{
	return spInstance;
}

// �A�j���[�V�����̐؂�ւ�
void CDragon::ChangeAnimation(EDragonAnimType type)
{
	if (!(EDragonAnimType::None < type && type < EDragonAnimType::Num)) return;
	DragonData::AnimData data = DragonData::GetAnimData((int)type);
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
		// �퓬���
	case EState::eBattle:
		UpdateBattle();
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
	mpAttackMouthCol->Update();

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

	// �Փ˂����R���C�_�[���U������p�̃R���C�_�[�ł����
	if (self == mpAttackMouthCol)
	{
		CCharaBase* chara = dynamic_cast<CCharaBase*>(other->Owner());
		// ����̃R���C�_�[�̎����傪�L�����ł����
		if (chara != nullptr)
		{
			// �_���[�W��^����
			CPlayer* player = CPlayer::Instance();
			int atk = Status().atk;
			int def = player->Status().def;
			float motionvalue = GetMotionValue();
			// �_���[�W�v�Z
			int damage = player->TakePlayerToDamage(atk, def, motionvalue);

			// ���ɍU���ς݂̃L�����łȂ����
			if (!IsAttackHitObj(chara))
			{
				// �_���[�W��^����
				chara->TakeDamage(damage);

				// �U���ς݂̃��X�g�ɒǉ�
				AddAttackHitObj(chara);
			}
		}
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

// �_���[�W����
void CDragon::TakeDamage(int damage)
{
	// �_���[�W��HP������
	mStatus.hp -= damage;

	// �{���ԂŖ�����΁A�_���[�W���{��l����
	if (!mIsAngry)
	{
		mAngryValue += damage;
		// �{��l������傫���Ȃ����ꍇ�A�{���Ԃֈڍs
		if (mAngryValue >= mAngryStandardValue)
		{
			mIsAngry = true;
			mAngryValue = mAngryStandardValue;
			ChangeAnimation(EDragonAnimType::eScream);
		}
	}
	// �{���ԂŗL��΁A�_���[�W�̔����̒l�����{��l������
	else
	{
		mAngryValue -= damage / 2;
		// �{��l��0�ȉ��ɂȂ����ꍇ�A�{���Ԃ�����
		if (mAngryValue <= 0)
		{
			mIsAngry = false;
			mAngryValue = 0;
		}
	}

	if (mStatus.hp <= 0)
	{
		// ���S����
	}
	else
	{
		// �̂�����Ȃǂ̔�e����
	}
}

// �U���J�n
void CDragon::AttackStart()
{
	CCharaBase::AttackStart();
	// �U�����n�܂�����A�U������p�̃R���C�_�[���I���ɂ���
	switch (AnimationIndex())
	{
	case (int)EDragonAnimType::eAttackMouth:// ���݂��U��
		mpAttackMouthCol->SetEnable(true);
		break;
	case (int)EDragonAnimType::eAttackHand:// ��ъ|����U��
		break;
	}
}

// �U���I��
void CDragon::AttackEnd()
{
	CCharaBase::AttackEnd();
	// �U�����I���΁A�U������p�̃R���C�_�[���I�t�ɂ���
	mpAttackMouthCol->SetEnable(false);
}