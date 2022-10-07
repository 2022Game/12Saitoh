#include<vector>
#include "CTriangle.h"
#include "CMaterial.h"
#ifndef CMODEL_H
#define CMEDEL_H
/*
���f���N���X
���f���f�[�^�̓��͂�\��
*/
class CModel
{
private:
	//�}�e���A���|�C���^�̉ϒ��z��
	std::vector<CMaterial*> mpMaterials;
	//�O�p�`�̉ϒ��z��
	std::vector<CTriangle> mTriangles;
public:
	//�`��
	void Render();
	//���f���t�@�C���̓���
	//Load(���f���t�@�C����, �}�e���A���t�@�C����)
	void Load(char* obj, char* mtl);
	~CModel();
};
#endif