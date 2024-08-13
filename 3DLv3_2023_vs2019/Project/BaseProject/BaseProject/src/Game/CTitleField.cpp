#include "CTitleField.h"
#include "CModel.h"

// �R���X�g���N�^
CTitleField::CTitleField()
	: CObjectBase(ETag::eField, ETaskPriority::eBackground)
{
	mpModel = CResourceManager::Get<CModel>("TitleField");
}

// �f�X�g���N�^
CTitleField::~CTitleField()
{
}

// �X�V����
void CTitleField::Update()
{
}

// �`�揈��
void CTitleField::Render()
{
	mpModel->Render(Matrix());
}