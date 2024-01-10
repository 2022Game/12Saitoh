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
	{ "",	true,	140.0f,	0.0f	},	// �A�C�h��
	{ "",	false,	900.0f,	0.0f	},	// �A�C�h��2
	{ "",	false,	300.0f,	0.0f	},	// �A�C�h��3
	{ "",	true,	90.0f,	0.0f	},	// ����
	{ "",	true,	42.0f,	0.0f	},	// ����
	{ "",	false,	66.0f,	0.0f	},	// ���݂�
	{ "",	false,	138.0f,	0.0f	},	// �����~��
	{ "",	false,	120.0f,	0.0f	},	// �K���U��	
	{ "",	false,	180.0f,	0.0f	},	// ���K
	{ "",	false,	45.0f,	0.0f	},	// ����(�E)
	{ "",	false,	45.0f,	0.0f	},	// ����(��)
	{ "",	false,	180.0f,	0.0f	},	// ���S
	{ "",	true,	135.0f,	0.0f	},	// ��s
	{ "",	true,	45.0f,	0.0f	},	// ��s�n��
	{ "",	false,	183.0f,	0.0f	},	// ��s���U��
	{ "",	false,	336.0f,	0.0f	},	// ��s���u���X
	{ "",	false,	63.0f,	0.0f	},	// ��s������
	{ "",	false,	183.0f,	0.0f	},	// ��s�����S
};

// �R���X�g���N�^
CEnemy::CEnemy()
	: CXCharacter(ETag::eEnemy, ETaskPriority::eEnemy)
	, mElapsedTime(0.0f)
	, mIdleTime(0)
{
	// �C���X�^���X�̐ݒ�
	spInstance = this;

	// ���f���f�[�^�擾
	CModelX* model = CResourceManager::Get<CModelX>("Enemy");

	// �A�j���[�V�����̕���
	model->SeparateAnimationSet(0, 0, 40, "Idle");				// �A�C�h��1
	model->SeparateAnimationSet(0, 43, 343, "Idle2");			// �A�C�h��2
	model->SeparateAnimationSet(0, 1004, 1104, "Idle3");		// �A�C�h��3
	model->SeparateAnimationSet(0, 614, 644, "Walk");			// ����
	model->SeparateAnimationSet(0, 646, 660, "Run");			// ����
	model->SeparateAnimationSet(0, 343, 365, "Attack_Bite");	// ���݂�
	model->SeparateAnimationSet(0, 366, 412, "Attack_Scratching");// �����~��
	model->SeparateAnimationSet(0, 476, 516, "Attack_Tail");	// �K���U��
	model->SeparateAnimationSet(0, 414, 474, "Roar");			// ���K
	model->SeparateAnimationSet(0, 518, 533, "Fear_Right");		// ����(�E)
	model->SeparateAnimationSet(0, 535, 550, "Fear_Left");		// ����(��)
	model->SeparateAnimationSet(0, 552, 612, "Death");			// ���S
	model->SeparateAnimationSet(0, 662, 707, "Fly");			// ��s
	model->SeparateAnimationSet(0, 717, 730, "FlyStart");		// ��s�n��
	model->SeparateAnimationSet(0, 707, 768, "Fly_Attack");		// ��s���U��
	model->SeparateAnimationSet(0, 771, 863, "Fly_Breath");		// ��s���u���X
	model->SeparateAnimationSet(0, 885, 906, "Fly_Fear");		// ��s������
	model->SeparateAnimationSet(0, 907, 968, "Fly_Death");		// ��s�����S


	// CXCharacter�̏�����
	Init(model);

	// �ŏ��͑ҋ@�̃A�j���[�V�������Đ�
	ChangeAnimation(EAnimType::eIdle);
	// �X�e�[�^�X�̎擾
	mStatus = ENEMY_STATUS[0];

	// ��Ԃ̐ݒ�
	mState = EState::eIdle;
	// �R���C�_�[�̐���
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
	// ��Ԃɍ��킹�āA�X�V������؂�ւ���
	switch (mState)
	{
		// �A�C�h�����
	case EState::eIdle:
		Update_Idle();
		break;
		// �ړ����
	case EState::eMove:
		Update_Move();
		break;
		// �U�����
	case EState::Attack:
		Update_Attack();
		break;
		// ���S���
	case EState::eDeath:
		Update_Death();
		break;
	}

	CXCharacter::Update();
	ColliderUpdate();
#ifdef _DEBUG
	auto enemydata = Instance();
	CVector enemy = Position();
	CDebugPrint::Print("X : %1f, Y : %1f, Z : %1f\n", enemy.X(), enemy.Y(), enemy.Z());

	if (CInput::PushKey('1')) ChangeAnimation(EAnimType::eIdle);
	else if (CInput::PushKey('2')) ChangeAnimation(EAnimType::eIdle2);
	else if (CInput::PushKey('3')) ChangeAnimation(EAnimType::eIdle3);
	else if (CInput::PushKey('4')) ChangeAnimation(EAnimType::eWalk);
	else if (CInput::PushKey('5')) ChangeAnimation(EAnimType::eRun);
	else if (CInput::PushKey('6')) ChangeAnimation(EAnimType::eAttack_Bite);
	else if (CInput::PushKey('7')) ChangeAnimation(EAnimType::eAttack_Scratching);
	else if (CInput::PushKey('8')) ChangeAnimation(EAnimType::eAttack_Tail);
	else if (CInput::PushKey('9')) ChangeAnimation(EAnimType::eRoar);
	if (CInput::Key('T') && CInput::PushKey('1')) ChangeAnimation(EAnimType::eFear_Right);
	else if (CInput::Key('T') && CInput::PushKey('2')) ChangeAnimation(EAnimType::eFear_Left);
	else if (CInput::Key('T') && CInput::PushKey('3')) ChangeAnimation(EAnimType::eDeath);
	else if (CInput::Key('T') && CInput::PushKey('4')) ChangeAnimation(EAnimType::eFly);
	else if (CInput::Key('T') && CInput::PushKey('5')) ChangeAnimation(EAnimType::eFly_Attack);
	else if (CInput::Key('T') && CInput::PushKey('6')) ChangeAnimation(EAnimType::eFly_Breath);
	else if (CInput::Key('T') && CInput::PushKey('7')) ChangeAnimation(EAnimType::eFly_Fear);
	else if (CInput::Key('T') && CInput::PushKey('8')) ChangeAnimation(EAnimType::eFly_Death);
	else if (CInput::Key('T') && CInput::PushKey('9')) ChangeAnimation(EAnimType::eFlyStart);
#endif
}

// ���S����
void CEnemy::Update_Death()
{

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