#include "CEnemy.h"
#include "CEffect.h"
#include "CCollisionManager.h"
#include "PlayerData.h"

CEnemy* CEnemy::spInstance = nullptr;

// �R���X�g���N�^
CEnemy::CEnemy()
	: CXCharacter(ETag::eEnemy, ETaskPriority::eEnemy)
{
	// �C���X�^���X�̐ݒ�
	spInstance = this;

	// ���f���f�[�^�ǂݍ���
	CModelX* model = CResourceManager::Get<CModelX>("Player");

	// CXCharacter�̏�����
	Init(model);
	// �X�e�[�^�X�̎擾
	mStatus = ENEMY_STATUS[0];
}

//�X�V����
void CEnemy::Update()
{
}

CEnemy* CEnemy::Instance()
{
	return spInstance;
}