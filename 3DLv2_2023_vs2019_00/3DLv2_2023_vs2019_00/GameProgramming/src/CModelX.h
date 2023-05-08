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
class CMesh;           //CMesh�N���X�̐錾
class CMaterial;       //�}�e���A���̐錾
class CSkinWeights;    //�X�L���E�F�C�g�N���X�̐錾
class CAnimationSet;   //�A�j���[�V�����Z�b�g�N���X�̐錾

/*
CModel
X�t�@�C���`����3D���f���f�[�^���v���O�����ŔF������
*/
class CModelX {
	friend CModelXFrame;
	friend CMesh;
	friend CAnimationSet;
private:
	std::vector<CModelXFrame*> mFrame;  //�t���[���̔z��
	std::vector<CAnimationSet*> mAnimationSet;	//�A�j���[�V�����Z�b�g�z��
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

	//�g�[�N�����Ȃ��Ȃ�����true
	bool EOT();

	//�m�[�h�̓ǂݔ�΂�
	void SlipNode();
	//�t�@�C���ǂݍ���
	void Load(char* file);

	//�`�揈��
	void Render();
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

	//�`�揈��
	void Render();
};

//Mesh�N���X�̒�`
class CMesh {
	friend CModelX;
	friend CModelXFrame;
	friend CSkinWeights;
private:
	int mFaceNum;          //�ʐ�
	int mVertexNum;	   	   //���_��
	int mNormalNum;        //�@����
	int mMaterialNum;      //�}�e���A����
	int mMaterialIndexNum; //�}�e���A���ԍ���(�ʐ�)
	int* mpMaterialIndex;  //�}�e���A���ԍ�
	int* mpVertexIndex;    //�ʂ��\�����钸�_�C���f�b�N�X

	std::vector<CSkinWeights*> mSkinWeights;	//�X�L���E�F�C�g
	std::vector<CMaterial*> mMaterial; //�}�e���A���f�[�^
	CVector* mpVertex;	   //���_�f�[�^
	CVector* mpNormal;     //�@���x�N�g��
	CSkinWeights* mpSkinWeghts;

public:
	//�R���X�g���N�^
	CMesh();
	//�f�X�g���N�^
	~CMesh();

	//�ǂݍ��ݏ���
	void Init(CModelX* model);

	//�`�揈��
	void Render();
};

/*
CSkinWeights
�X�L���E�F�C�g�N���X
*/
class CSkinWeights
{
	friend CModelX;
	friend CMesh;
private:
	char* mpFrameName;	//�t���[����
	int mFrameIndex;	//�t���[���ԍ�
	int mIndexNum;		//���_�ԍ���
	int* mpIndex;		//���_�ԍ��z��
	float* mpWeight;	//���_�E�F�C�g�z��
	CMatrix mOffset;	//�I�t�Z�b�g�}�g���b�N�X

public:
	CSkinWeights(CModelX* model);
	~CSkinWeights();
	const int& FrameIndex();
	const CMatrix& Offset();
};

/*
CAnimationSet
�A�j���[�V�����Z�b�g
*/
class CAnimationSet
{
private:
	char* mpName;	//�A�j���[�V�����Z�b�g��

public:
	CAnimationSet(CModelX* model);
	~CAnimationSet();
};
#endif