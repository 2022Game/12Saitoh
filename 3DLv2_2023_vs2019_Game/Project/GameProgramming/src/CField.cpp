#include "CField.h"
#include "CTaskManager.h"

//�R���X�g���N�^
CField::CField()
{
	//�|�[�Y�̎�ނ�ݒ�
	SetPauseType(TaskPauseType::eGame);
	//�^�X�N�̗D��x�ݒ�
	mPriority = (int)TaskPriority::eBackGround;
	//�^�X�N�}�l�[�W���Ɏ��M��ǉ�
	CTaskManager::Instance()->Add(this);

	mModel = new CModel();
	//���f���̓���
	mModel->Load("res\\Graund.obj", "res\\Graund.mtl");
	//�ǂݍ��񂾔w�i���f���f�[�^�ŃR���C�_�𐶐�
	mColliderMesh.Set(nullptr, nullptr, mModel);
}

//�f�X�g���N�^
CField::~CField()
{
	//�^�X�N�}�l�[�W�����玩�M����菜��
	CTaskManager::Instance()->Remove(this);

	//�w�i���f���f�[�^��j��
	delete mModel;
	mModel = nullptr;
}

//�X�V����
void CField::Update()
{
}

//�`�揈��
void CField::Render()
{
	if (mModel == nullptr) return;
	//�w�i���f����`��
	mModel->Render();
}