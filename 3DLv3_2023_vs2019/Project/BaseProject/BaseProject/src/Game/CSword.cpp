#include "CSword.h"
#include "CPlayer.h"
#include "CDragon.h"
#include "CColliderCapsule.h"

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
				mpSwordEffect = new CNormalSwordEffect
				(
					hit.cross,
					CVector::zero,
					CQuaternion(0.0f, 0.0f, 0.0f).Matrix()
				);
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