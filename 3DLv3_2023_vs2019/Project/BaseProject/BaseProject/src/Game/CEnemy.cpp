#include "CEnemy.h"
#include "CEffect.h"
#include "CCollisionManager.h"
#include "PlayerData.h"

CEnemy* CEnemy::spInstance = nullptr;


// �G�̃A�j���[�V�����f�[�^�̃e�[�u��
const CEnemy::AnimData CEnemy::ANIM_DATA[] =
{
	{ "",									true,	0.0f	},	// T�|�[�Y
	{ "Character\\Enemy\\anim\\Idle.x",		true,	334.0f	},	// �A�C�h��
};

// �R���X�g���N�^
CEnemy::CEnemy()
	: CXCharacter(ETag::eEnemy, ETaskPriority::eEnemy)
{
	// �C���X�^���X�̐ݒ�
	spInstance = this;

	// ���f���f�[�^�擾
	CModelX* model = CResourceManager::Get<CModelX>("Enemy");

	// �e�[�u�����̃A�j���[�V�����f�[�^��ǂݍ���
	//int size = ARRAY_SIZE(ANIM_DATA);
	//for (int i = 0; i < size; i++)
	//{
	//	const AnimData& data = ANIM_DATA[i];
	//	if (data.path.empty()) continue;
	//	model->AddAnimationSet(data.path.c_str());
	//}

	// CXCharacter�̏�����
	Init(model);

	// �ŏ��͑ҋ@�̃A�j���[�V�������Đ�
	//ChangeAnimation(EAnimType::eIdle);
	// �X�e�[�^�X�̎擾
	mStatus = ENEMY_STATUS[0];
}

// �A�j���[�V�����̐؂�ւ�
void CEnemy::ChangeAnimation(EAnimType type)
{
	if (!(EAnimType::None < type && type < EAnimType::Num)) return;
	AnimData data = ANIM_DATA[(int)type];
	CXCharacter::ChangeAnimation((int)type, data.loop, data.frameLength, 0.0f);
}

//�X�V����
void CEnemy::Update()
{
	CXCharacter::Update();

#ifdef _DEBUG
	auto enemydata = Instance();
	CVector enemy = Position();
	CDebugPrint::Print("X : %1f, Y : %1f, Z : %1f", enemy.X(), enemy.Y(), enemy.Z());
#endif
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