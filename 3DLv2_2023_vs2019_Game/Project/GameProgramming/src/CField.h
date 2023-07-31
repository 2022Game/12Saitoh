#pragma once
#include "CTask.h"
#include "CModel.h"
#include "CColliderMesh.h"

//�t�B�[���h�p�N���X
class CField : public CTask
{
public:
	//�R���X�g���N�^
	CField();
	//�f�X�g���N�^
	~CField();

	//�X�V����
	void Update();
	//�`�揈��
	void Render();

private:
	CModel* mModel;	//�t�B�[���h�̃��f���f�[�^
	CColliderMesh mColliderMesh;	//�t�B�[���h�̃R���C�_
};