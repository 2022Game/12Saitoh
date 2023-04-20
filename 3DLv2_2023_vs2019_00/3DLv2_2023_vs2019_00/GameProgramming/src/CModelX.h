#ifndef CMODELX_H
#define CMODELX_H
#include <vector>      //vector�N���X�̃C���N���[�h
#include "CMatrix.h"   //�}�g���N�X�N���X�̃C���N���[�h
#include "CVector.h"   //Vector�N���X�̃C���N���[�h

#define MODEL_FILE "res\\sample.blend.x"
//�̈������}�N����
#define SAFE_DELETE_ARRA(a) {if(a) delete[] a; a = nullptr;}

class CModelX;         //CModel�N���X�̐錾
class CModelXFrame;    //CModelXFrame�N���X�̐錾
class CMesh;           //CMesh�N���X�̒�`��ǉ�����

/*
CModel
X�t�@�C���`����3D���f���f�[�^���v���O�����ŔF������
*/
class CModelX {
	friend CModelXFrame;
	friend CMesh;
private:
	std::vector<CModelXFrame*> mFrame;  //�t���[���̔z��
	char* mpPointer;   //�ǂݍ��݈ʒu
	char mToken[1024]; //���o�����P��̗̈�

	//c����؂蕶���Ȃ�true��Ԃ�
	bool IsDelimiter(char c);
public:
	//�R���X�g���N�^
	CModelX();
	//�f�X�g���N�^
	~CModelX();

	//�P��̎��o��
	char* GetToken();
	char* Token();

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
	CMesh* mpMesh;  //Mesh�f�[�^
	CMatrix mTransformMatrix;  //�ϊ��s��
	char* mpName;  //�t���[�����O
	int mIndex;  //�t���[���ԍ�
public:
	//�R���X�g���N�^
	CModelXFrame(CModelX* model);
	//�f�X�g���N�^
	~CModelXFrame() ;
};

//Mesh�N���X�̒�`
class CMesh {
	friend CModelX;
	friend CModelXFrame;
private:
	int mFaceNum;       //�ʐ�
	int mVertexNum;		//���_��
	int* mpVertexIndex; //�ʂ��\�����钸�_�C���f�b�N�X
	CVector* mpVertex;	//���_�f�[�^

public:
	//�R���X�g���N�^
	CMesh();
	//�f�X�g���N�^
	~CMesh();

	//�ǂݍ��ݏ���
	void Init(CModelX* model);
};
#endif
