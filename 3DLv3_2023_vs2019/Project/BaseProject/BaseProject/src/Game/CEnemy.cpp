#include "CEnemy.h"
#include "CEffect.h"
#include "CCollisionManager.h"
#include "PlayerData.h"
#include "CColliderSphere.h"
#include "CPlayer.h"

CEnemy* CEnemy::spInstance = nullptr;

// �G�̃A�j���[�V�����f�[�^�̃e�[�u��
const CEnemy::AnimData CEnemy::ANIM_DATA[] =
{
	{ "",	true,	0.0f,	0.0f	},	// T�|�[�Y
	{ "",	true,	334.0f,	0.0f	},	// �A�C�h��
};

// �R���X�g���N�^
CEnemy::CEnemy()
	: CXCharacter(ETag::eEnemy, ETaskPriority::eEnemy)
{
	// �C���X�^���X�̐ݒ�
	spInstance = this;

	// ���f���f�[�^�擾
	CModelX* model = CResourceManager::Get<CModelX>("Enemy");

	// �A�j���[�V�����̕���
	model->SeparateAnimationSet(0, 0, 40, "Idle");

	// CXCharacter�̏�����
	Init(model);

	// �ŏ��͑ҋ@�̃A�j���[�V�������Đ�
	ChangeAnimation(EAnimType::eIdle);
	// �X�e�[�^�X�̎擾
	mStatus = ENEMY_STATUS[0];

	mpHeadCol = new CColliderSphere(this, ELayer::eDamageCol, 0.2);
	mpHeadCol->SetCollisionLayers({ ELayer::eAttackCol });
	mpHeadCol->SetCollisionTags({ ETag::ePlayer });

	const CMatrix* headcol = GetFrameMtx("Bip01_Bip01_Head");
	mpHeadCol->SetAttachMtx(headcol);
}

// �A�j���[�V�����̐؂�ւ�
void CEnemy::ChangeAnimation(EAnimType type)
{
	if (!(EAnimType::None < type && type < EAnimType::Num)) return;
	AnimData data = ANIM_DATA[(int)type];
	CXCharacter::ChangeAnimation((int)type, data.loop, data.frameLength, data.motionValue);
}

// �R���C�_�[�̍X�V����
void CEnemy::ColliderUpdate()
{
	mpHeadCol->Update();
}

//�X�V����
void CEnemy::Update()
{
	CXCharacter::Update();
	ColliderUpdate();

#ifdef _DEBUG
	auto enemydata = Instance();
	CVector enemy = Position();
	CDebugPrint::Print("X : %1f, Y : %1f, Z : %1f\n", enemy.X(), enemy.Y(), enemy.Z());

#endif
}

// �Փˏ���
void CEnemy::Collision(CCollider* self, CCollider* other, const CHitInfo& hit)
{
//	if (self == mpAttackCol)
//	{
//		CPlayer* player = dynamic_cast<CPlayer*>(other->Owner());
//		if (player != nullptr)
//		{
//			int atk = mStatus.atk;
//			int def = player->Status().def;
//			int motion = mMotionValue;
//			int damage = TakePlayerToDamage(atk, def, motion);
//			player->TakeDamage(damage);
//		}
//	}
}

// �`�揈��
void CEnemy::Render()
{
	CXCharacter::Render();
}

CEnemy* CEnemy::Instance()
{
	return spInstance;
}