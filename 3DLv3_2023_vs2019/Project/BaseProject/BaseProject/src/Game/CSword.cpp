#include "CSword.h"
#include "CPlayer.h"
#include "CDragon.h"
#include "CColliderCapsule.h"
#define NORMALATTACK_EFFECTLENGTH 40.0f

// �R���X�g���N�^
CSword::CSword()
{
	mpSword = CResourceManager::Get<CModel>("Sword");

	mpSwordCollider = new CColliderCapsule(this, ELayer::eAttackCol,
		CVector(0.0f, 0.0f, 0.0f), CVector(0.0f, 0.0f, 16.0f), 1.5f);
	mpSwordCollider->SetCollisionLayers({ ELayer::eDamageCol });
	mpSwordCollider->SetCollisionTags({ ETag::eEnemy });

	// �ŏ��͍U������p�̃R���C�_�[���I�t�ɂ��Ă���
	mpSwordCollider->SetEnable(false);
}

// �f�X�g���N�^
CSword::~CSword()
{
	SAFE_DELETE(mpSwordCollider);
}

// �X�V����
void CSword::Update()
{
	// �[����ԂƔ�����ԂŐe�̃{�[����ύX�^��
	if (CPlayer::Instance()->IsDrawn())
	{
		SetAttachMtx(CPlayer::Instance()->GetFrameMtx("Armature_weapon_r"));
	}
	else
	{
		SetAttachMtx(CPlayer::Instance()->GetFrameMtx("Armature_sword_holder"));
	}
}

// �Փˏ���
void CSword::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
	// �Փ˂��������̃R���C�_�[���U������p�̃R���C�_�[�ł����
	if (self == mpSwordCollider)
	{
		CCharaBase* chara = dynamic_cast<CCharaBase*>(other->Owner());
		// ����̃R���C�_�[�̎����傪�L�����ł����
		if (chara != nullptr)
		{
			// ���ɍU���ς݂̃L�����łȂ���΁A
			if (!IsAttackHitObj(chara))
			{
				// �_���[�W�v�Z
				CPlayer* player = CPlayer::Instance();
				CDragon* dragon = CDragon::Instance();
				int atk = player->Status().atk;
				int def = dragon->Status().def;
				float motionvalue = player->GetMotionValue();
				int damage = dragon->TakeEnemyToDamage(atk, def, motionvalue, other->GetMeat());
				// �_���[�W��^����
				chara->TakeDamage(damage);

				// ���C�𑝉�����
				player->UpTouki();

				// �U���ς݃��X�g�ɒǉ�
				AddAttackHitObj(chara);

				// �U���G�t�F�N�g�𐶐�
				CreateEffect(hit);
			}
		}
	}
}

// �`�揈��
void CSword::Render()
{
	mpSword->Render(Matrix());
}

// ����̍s��擾
CMatrix CSword::Matrix() const
{
	// ��Ɏ����Ă��Ȃ����́A�������g�̍s���Ԃ�
	if (mpAttachMtx == nullptr)
	{
		return CTransform::Matrix();
	}
	// ��Ɏ����Ă��鎞�́A�A�^�b�`���Ă���s���Ԃ�
	else 
	{
		CMatrix sm;
		CMatrix rm;
		sm.Scale(12.0f, 12.0f, 12.0f);
		// �[����ԂƔ�����Ԃ����肵�A�`��̒������s��
		if (CPlayer::Instance()->IsDrawn()) //����
		{
			//rm.RotateZ(180.0f);
			rm = rm.RotateY(-90.0f);
			return rm * sm * (*mpAttachMtx);
		}
		else //�[��
		{
			rm.RotateX(-90.0f);
			return rm * sm * (*mpAttachMtx);
		}
	}
}

// �U���J�n
void CSword::AttackStart()
{
	CWeapon::AttackStart();
	// �U�����n�܂�����A�U������p�̃R���C�_�[���I���ɂ���
	mpSwordCollider->SetEnable(true);
}

// �U���I��
void CSword::AttackEnd()
{
	CWeapon::AttackEnd();
	// �U�����I���΁A�U������p�̃R���C�_�[���I�t�ɂ���
	mpSwordCollider->SetEnable(false);
}

void CSword::CreateNormalEffect(const CHitInfo& hit)
{
	CPlayer* player = CPlayer::Instance();
	int animIndex = player->AnimationIndex();

	CVector side = player->VectorX();
	CVector up = player->VectorY();
	CVector forward = player->VectorZ();

	CVector pos = hit.cross;
	CVector dir;
	float length = NORMALATTACK_EFFECTLENGTH;

	switch (animIndex)
	{
	case (int)EAnimType::eNormalAttack1_1:// �ʏ�U��1_1
		dir = (side + up).Normalized();
		break;
	case (int)EAnimType::eNormalAttack1_2:// �ʏ�U��1_2
		dir = -(side + forward * 0.25f).Normalized();
		break;
	case (int)EAnimType::eNormalAttack1_3:// �ʏ�U��1_3
		// 1�x�ڂ̍U����
		if (NORMALATTACK1_3_COLLIDER > player->GetAnimationFrame())
		{
			dir = -(-side + up + forward * 0.5f).Normalized();
		}
		// 2�x�ڂ̍U����
		else
		{
			dir = -(side + up + forward * 0.5f).Normalized();
		}
		break;
	}
	// �a���G�t�F�N�g�𐶐�
	new CNormalSwordEffect
	(
		pos + dir * length,	// �G�t�F�N�g�̎n�_
		pos - dir * length // �G�t�F�N�g�̏I�_
	);
}

// �G�t�F�N�g���쐬
void CSword::CreateEffect(const CHitInfo& hit)
{
	CPlayer* player = CPlayer::Instance();
	int animIndex = player->AnimationIndex();

	// �ʏ�U���A�W�����v�U�����̃G�t�F�N�g
	if (player->GetState() == CPlayer::EState::eAttack)
	{
		switch (animIndex)
		{
		case (int)EAnimType::eNormalAttack1_1:// �ʏ�U��1_1
		case (int)EAnimType::eNormalAttack1_2:// �ʏ�U��1_2
		case (int)EAnimType::eNormalAttack1_3:// �ʏ�U��1_3
			CreateNormalEffect(hit);
			break;
		case (int)EAnimType::eAirAttack1_1:// �󒆍U��1_1
			mpAirEffect = new CAirAttackEffect(hit.cross);
			mpAirEffect->Rotate(-230.0f, 0.0f, 0.0f);
			break;
		case (int)EAnimType::eAirAttack1_2:// �󒆍U��1_2
			mpAirEffect = new CAirAttackEffect(hit.cross);
			mpAirEffect->Rotate(30.0f, 0.0f, 0.0f);
			break;
		case (int)EAnimType::eAirAttack1_3:// �󒆍U��1_3
			mpAirEffect = new CAirAttackEffect(hit.cross);
			mpAirEffect->Rotate(180.0f, 0.0f, 0.0f);
			break;
		case (int)EAnimType::eAirAttack1_4:// �󒆍U��1_4
			mpAirEffect = new CAirAttackEffect(hit.cross);
			mpAirEffect->Rotate(350.0f, 0.0f, 0.0f);
			break;
		case (int)EAnimType::eAttack_Up:// �W�����v�U��
			mpAirEffect = new CAirAttackEffect(hit.cross);
			mpAirEffect->Rotate(230.0f, 0.0f, 0.0f);
			break;
		}
	}
	// ���Z���̃G�t�F�N�g
	else if(player->GetState() == CPlayer::EState::eSpecalMove)
	{
		switch (animIndex)
		{
		case (int)EAnimType::eParryAttack:// �J�E���^�[�U��(�㓬�Z)
			if (COUNTERATTACK_START >= player->GetAnimationFrame())
			{
				mpCounterEffect1 = new CCounterEffect(hit.cross);
			}
			break;
		case (int)EAnimType::ePowerAttack:// �����Z�U��
			break;
		}
	}
}