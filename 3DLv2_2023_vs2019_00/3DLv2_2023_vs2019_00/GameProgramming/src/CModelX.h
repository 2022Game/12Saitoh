#ifndef CMODELX_H
#define CMODELX_H

#define MODEL_FILE "res\\sample.blend.x"

//�̈������}�N����
#define SAFE_DELETE_ARRA(a) {if(a) delete[] a; a = nullptr;}
/*
CModel
X�t�@�C���`����3D���f���f�[�^���v���O�����ŔF������
*/
class CModelX {
private:
	char* mpPointer;   //�ǂݍ��݈ʒu
	char mToken[1024]; //���o�����P��̗̈�
public:
	//�R���X�g���N�^
	CModelX();

	//�t�@�C���ǂݍ���
	void Load(char* file);
};
#endif
