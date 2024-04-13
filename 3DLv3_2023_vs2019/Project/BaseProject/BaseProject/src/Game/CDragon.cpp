#include "CDragon.h"
#include "Maths.h"
#include "CPlayer.h"
#include "Primitive.h"
#include "CColliderSphere.h"
#include "CColliderLine.h"
#include "CDebugPrint.h"
#include "CFlamethrower.h"
#include "Global.h"

CDragon* CDragon::spInstance = nullptr;

// �R���X�g���N�^
CDragon::CDragon()
	: CXCharacter(ETag::eEnemy, ETaskPriority::eEnemy)
	, mMoveSpeed(CVector::zero)
	, mIsGrounded(true)
	, mIsAngry(false)
	, mAngryStandardValue(0)
	, mAngryValue(0)
	, mRandSave(0)
	, mBatteleStep(0)
	, mElapsedTime(0.0f)
	, mAngryElapsedTime(0.0f)
	, mChaseElapsedTime(0.0f)
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
	mpColliderLine->SetCollisionLayers({ ELayer::eField });
	// �ǂƂ̓����蔻��p�R���C�_�̐ݒ�
	mpColliderLine2 = new CColliderLine
	(
		this, ELayer::eField,
		CVector(-150.0f, 200.0f, 0.0f),
		CVector(150.0f, 200.0f, 0.0f)
	);
	mpColliderLine2->SetCollisionLayers({ ELayer::eField });
	mpColliderLine3 = new CColliderLine
	(
		this, ELayer::eField,
		CVector(0.0f, 200.0f, -300.0f),
		CVector(0.0f, 200.0f, 500.0f)
	);
	mpColliderLine3->SetCollisionLayers({ ELayer::eField });

	// �R���C�_�[�̐���
	// �v���C���[�Ƃ̉����߂��p�R���C�_�[
	mpBodyCol = new CColliderSphere(this, ELayer::eEnemy, 200.0f);
	mpBodyCol->SetCollisionLayers({ ELayer::ePlayer });
	mpBodyCol->SetCollisionTags({ ETag::ePlayer });

	// �_���[�W���󂯂�p�̃R���C�_�[
	mpDamageCol = new CColliderSphere(this, ELayer::eDamageCol, 500.0f);
	mpDamageCol->SetCollisionLayers({ ELayer::eAttackCol });
	mpDamageCol->SetCollisionTags({ ETag::eWeapon });
	mpDamageCol->SetMeat(100);

	// �U���p�̃R���C�_�[
	mpAttackMouthCol = new CColliderSphere(this, ELayer::eAttackCol, 1.5f);
	mpAttackMouthCol->SetCollisionLayers({ ELayer::eDamageCol });
	mpAttackMouthCol->SetCollisionTags({ ETag::ePlayer });
	// �R���C�_�[���A�^�b�`
	const CMatrix* attackcol = GetFrameMtx("Armature_Tongue02");
	mpAttackMouthCol->SetAttachMtx(attackcol);

	// �ŏ��͍U������p�̃R���C�_�[�̓I�t�ɂ��Ă���
	mpAttackMouthCol->SetEnable(false);

	const CMatrix* flamemtx = GetFrameMtx("Armature_UpperMouth01");
	mpFlamethrower = new CFlamethrower
	(
		this, flamemtx,
		CVector(0.0f, -20.0f, 0.0f),
		CQuaternion(0.0f, 90.0f,0.0f).Matrix()
	);
}

// �f�X�g���N�^
CDragon::~CDragon()
{
	SAFE_DELETE(mpColliderLine);
	SAFE_DELETE(mpBodyCol);
	SAFE_DELETE(mpDamageCol);
	SAFE_DELETE(mpAttackMouthCol);
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

// �o�b�N�X�e�b�v�ł��邩�ǂ���
bool CDragon::IsBackStep() const
{
	// ���C�̊J�n�n�_
	CVector startPos = Position() + CVector(0.0f,50.0f,0.0f);
	// ���C�̏I���n�_
	CVector endPos = -VectorZ().Normalized() * FIELD_RADIUS;

	float outDist = 0.0f;
	// �t�B�[���h�ƃ��C�̓����蔻����s��
	gField->CollisionRay(startPos, endPos, &outDist);
	
	// �o�b�N�X�e�b�v������͈͂�����
	if (outDist > 150.0f) return true;

	return false;
}


//�v���C���[�Ƃ̋������擾
CDragon::EDistanceType CDragon::PlayerFromDistance()
{
	CVector playerPos = CPlayer::Instance()->Position();
	CVector enemyPos = Position();

	// �����̔���
	float distance = (playerPos - enemyPos).Length();
	// �����Ɣ��ʗp�̒l���ׂāA�����𔻕�
	if (distance >= 280.0f) mDistanceType = EDistanceType::eFar;// ������
	else if (distance >= 230.0f) mDistanceType = EDistanceType::eMedium;// ������
	else mDistanceType = EDistanceType::eNear;// �ߋ���

	// ���ʂ���������Ԃ�
	return mDistanceType;
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

	// �U�����܂��́A�o�b�N�X�e�b�v�ȊO�̎��͈ړ������֌�����
	if (mBatteleStep != 2 &&
		AnimationIndex() != (int)EDragonAnimType::eBackStep)
	{
		CVector current = VectorZ();
		CVector target = mMoveSpeed;
		target.Y(0.0f);
		target.Normalize();
		CVector forward = CVector::Slerp(current, target, 0.02f);
		Rotation(CQuaternion::LookRotation(forward));
	}

	CXCharacter::Update();
	mpAttackMouthCol->Update();

#ifdef _DEBUG
	// �h���S���̃X�e�[�^�X��\��
	CDebugPrint::Print("�G�X�e�[�^�X\n");
	CDebugPrint::Print("���x�� : %d\nHP : %d\n�U���� : %d\n�h��� : %d\n",
		mStatus.level, mStatus.hp, mStatus.atk, mStatus.def);

	// �v���C���[�Ƃ̋�����\��
	CVector pPos = CPlayer::Instance()->Position();
	CVector ePos = Position();
	float distance = (pPos - ePos).Length();
	CDebugPrint::Print("�v���C���[�Ƃ̋��� : %.1f\n", distance);

	if (mDistanceType == EDistanceType::eNear) CDebugPrint::Print("�ߋ���\n");
	else if (mDistanceType == EDistanceType::eMedium) CDebugPrint::Print("������\n");
	else CDebugPrint::Print("������\n");

	if (CInput::PushKey('1'))
	{
		int p = GetHPPercent();
		printf("%d\n", p);
	}

	if (IsBackStep()) CDebugPrint::Print("true\n");
	else CDebugPrint::Print("false\n");

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
	// �ǂƂ̓����蔻��
	if (self == mpColliderLine2)
	{
		if (other->Layer() == ELayer::eField)
		{
			mMoveSpeed.X(0.0f);
			// �߂荞�܂Ȃ��悤�ɒ���
			Position(Position() + hit.adjust);
		}
	}
	if (self == mpColliderLine3)
	{
		if (other->Layer() == ELayer::eField)
		{
			mMoveSpeed.Z(0.0f);
			// �߂荞�܂Ȃ��悤�ɒ���
			Position(Position() + hit.adjust);
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
#ifdef _DEBUG

	// ���씻��p
	Primitive::DrawSector(
		Position() + CVector(0.0f, 1.0, 0.0f),
		-EulerAngles(),
		-FOV_ANGLE,
		FOV_ANGLE,
		FOV_LANGE,
		CColor::red,
		45
	);
#endif
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