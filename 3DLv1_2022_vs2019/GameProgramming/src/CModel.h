#include<vector>
#include "CTriangle.h"
#ifndef CMODEL_H
#define CMEDEL_H
/*
���f���N���X
���f���f�[�^�̓��͂�\��
*/
class CModel
{
private:
	//�O�p�`�̉ϒ��z��
	std::vector<CTriangle> mTriangles;
public:
	//�`��
	void Render();
	//���f���t�@�C���̓���
	//Load(���f���t�@�C����, �}�e���A���t�@�C����)
	void Load(char* obj, char* mtl);
};
#endif