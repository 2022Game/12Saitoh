#ifndef CMODELX_H
#define CMODELX_H
#include <vector>      //vector�N���X�̃C���N���[�h
#include "CMatrix.h"   //�}�g���N�X�N���X�̃C���N���[�h

#define MODEL_FILE "res\\sample.blend.x"
//�̈������}�N����
#define SAFE_DELETE_ARRA(a) {if(a) delete[] a; a = nullptr;}

class CModelX;         //CModel�N���X�̐錾
class CModelXFrame;    //CModelXFrame�N���X�̐錾
/*
CModel
X�t�@�C���`����3D���f���f�[�^���v���O�����ŔF������
*/
class CModelX {
	friend CModelXFrame;
private:
	std::vector<CModelXFrame*> mFrame;  //�t���[���̔z��
	char* GetToken();  //�P��̎��o��
	char* mpPointer;   //�ǂݍ��݈ʒu
	char mToken[1024]; //���o�����P��̗̈�

	//c����؂蕶���Ȃ�true��Ԃ�
	bool IsDelimiter(char c);
public:
	//�R���X�g���N�^
	CModelX();
	//�f�X�g���N�^
	~CModelX();

	//�m�[�h�̓ǂݔ�΂�
	void SlipNode();
	//�t�@�C���ǂݍ���
	void Load(char* file);
};

//CModelXFrame�N���X�̒�`
class CModelXFrame {
	friend CModelX;
private:
	std::vector<CModelXFrame*> mChild; //�q�t���[���̔z��
	CMatrix mTransformMatrix;  //�ϊ��s��
	char* mpName;  //�t���[�����O
	int mIndex;  //�t���[���ԍ�
public:
	//�R���X�g���N�^
	CModelXFrame(CModelX* model);
	//�f�X�g���N�^
	~CModelXFrame() ;
};
#endif
