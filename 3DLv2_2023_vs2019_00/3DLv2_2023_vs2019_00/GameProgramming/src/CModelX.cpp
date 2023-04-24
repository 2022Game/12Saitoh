#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "CModelX.h"
#include "glut.h"

/*
IsDelimiter(c)
c��\t \r \n �X�y�[�X�Ȃǂ̋󔒕���
�܂��́A;�@" �Ȃǂ̕����ł����
��؂蕶���Ƃ���true��Ԃ�
*/
bool CModelX::IsDelimiter(char c)
{
	//isspace(c)
	//c���󔒕����΂�0�ȊO��Ԃ�
	if (isspace(c) != 0)
		return true;
	/*
	turchr(������,����)
	������ɕ������܂܂�Ă���΁A
	�������������փ|�C���^��Ԃ�
	������Ȃ�������NULL��Ԃ�
	*/
	if (strchr(",;\"", c) != NULL)
		return true;
	//��؂蕶���ł͂Ȃ�
	return false;
}

CModelX::CModelX()
	:mpPointer(nullptr)
{
	//mToken��������
	memset(mToken, 0, sizeof(mToken));
}

CModelX::~CModelX()
{
	if (mFrame.size() > 0)
	{
		delete mFrame[0];
	}
}

/*
GetToken
������f�[�^����A�P���1�擾����
*/
char* CModelX::GetToken()
{
	char* p = mpPointer;
	char* q = mToken;
	//�^�u(\t)��()���s(\r)(\n),;"�̋�؂蕶���ȊO�ɂȂ�܂œǂݔ�΂�
	while (*p != '\0' && IsDelimiter(*p))p++;
	if (*p == '{' || *p == '}')
		*q++ = *p++;
	else
	{
		while (*p != '\0' && !IsDelimiter(*p) && *p != '}')
			*q++ = *p++;
	}

	*q = '\0';    //mToken�̍Ō��\0����
	mpPointer = p; //���̓ǂݍ��ރ|�C���g���X�V����

	//����Token��//�̏ꍇ�́A�R�����g�Ȃ̂ŉ��s�܂œǂݔ�΂�
	/*
	strcmp(������1,������2)
	������P�ƕ�����2���������ꍇ�A0��Ԃ��܂��B
	������1�ƕ�����2���������Ȃ��ꍇ�A0�ȊO��Ԃ��܂��B
	*/
	if (!strcmp("//", mToken))
	{
		//���s�܂œǂݔ�΂�
		while (*p != '\0' && !strchr("\r\n", *p))p++;
		//�ǂݍ��݈ʒu�̍X�V
		mpPointer = p;
		//�P����擾����(�ċA�Ăяo��)
		return GetToken();
	}
	return mToken;
}

char* CModelX::Token()
{
	return mToken;
}

/*
SkipNode
�m�[�h��ǂݔ�΂�
*/
void CModelX::SlipNode()
{
	//�������I�������I��
	while (*mpPointer != '\0')
	{
		GetToken(); //���̒P��擾
		//{�����������烋�[�v�I��
		if (strchr(mToken, '{')) break;
	}
	int count = 1;
	//�������I��邩�A�J�E���g��0�ɂȂ�����I��
	while (*mpPointer != '\0' && count > 0)
	{
		GetToken();  //���̒P��擾
		//{��������ƃJ�E���g�A�b�v
		if (strchr(mToken, '{')) count++;
		//}��������ƃJ�E���g�_�E��
		else if (strchr(mToken, '}')) count--;
	}
}

void CModelX::Load(char* file)
{
	//�t�@�C���T�C�Y���擾����
	FILE* fp;

	fp = fopen(file, "rb"); //�t�@�C�����I�[�v������
	//�G���[�`�F�b�N
	if (fp == NULL)
	{
		printf("fopen error:%s\n", file);
		return;
	}
	printf("%s\n", file);
	//�t�@�C���̍Ō�ֈړ�
	fseek(fp, 0L, SEEK_END);
	//�t�@�C���T�C�Y�̎擾
	int size = ftell(fp);
	//�t�@�C���T�C�Y+1�o�C�g���̗̈���m��
	char* buf = mpPointer = new char[size + 1];
	//�t�@�C������3D���f���̃f�[�^��ǂݍ���
	//�t�@�C���̐擪�ֈړ�
	fseek(fp, 0L, SEEK_SET);
	//�m�ۂ����̈�Ƀt�@�C���T�C�Y���f�[�^��ǂݍ���
	fread(buf, size, 1, fp);
	//�Ō��\0��ݒ肷��(������̏I�[)
	buf[size] = '\0';
	//������̍Ō�܂ŌJ��Ԃ�
	while (*mpPointer != '\0')
	{
		GetToken(); //�P��̎擾
		//�P�ꂪFrame�̏ꍇ
		if (strcmp(mToken, "Frame") == 0)
		{
			//�t���[���𐶐�����
			new CModelXFrame(this);
		}
	}
	fclose(fp); //�t�@�C�����N���[�Y����

	SAFE_DELETE_ARRA(buf); //�m�ۂ����̈���������
}

/*
Render
���ׂẴt���[���̕`�揈�����Ăяo��
*/
void CModelX::Render()
{
	for (size_t i = 0; i < mFrame.size(); i++)
	{
		mFrame[i]->Render();
	}
}

/*
CModelXFrame
model:CModelX�C���X�^���X�ւ̃|�C���^
�v���[�����쐬����
�ǂݍ��ݒ���Frame��������΁A�t���[�����쐬���A
�q�t���[���ɒǉ�����
*/
CModelXFrame::CModelXFrame(CModelX* model)
	: mpMesh(nullptr)
	, mpName(nullptr)
	, mIndex(0)
{
	//���݂̃t���[���z��ɗv�f�����擾���ݒ肷��
	mIndex = model->mFrame.size();
	//CModelX�t���[���z��ɒǉ�����
	model->mFrame.push_back(this);
	//�ϊ��s���P�ʍs��ɂ���
	mTransformMatrix.Identity();
	//���̒P��(�t���[�����̗\��)���擾����
	model->GetToken();  //Frae name
	//�t���[�������G���A���m�ۂ���
	mpName = new char[strlen(model->mToken) + 1];
	//�t���[�����R�s�[����
	strcpy(mpName, model->mToken);
	//���̒P��({�̗\��)���擾����
	model->GetToken();//{
	//�������Ȃ��Ȃ�����I���
	while (*model->mpPointer != '\0')
	{
		//���̒P��擾
		model->GetToken(); //Frame
	    //}�̏ꍇ�͏I��
		if (strchr(model->mToken, '}'))break;
		//�V���ȃt���[���̏ꍇ�́A�q�t���[���ɒǉ�
		if (strcmp(model->mToken,"Frame") == 0)
		{
			//�t���[���𐶐����A�q�t���[���ɒǉ�
			mChild.push_back(new CModelXFrame(model));
		}
		else if (strcmp(model->mToken, "FrameTransformMatrix") == 0)
		{
			model->GetToken();  //{
			for (int i = 0; i < mTransformMatrix.Size(); i++)
			{
				//atof->������̃f�[�^��float�^�ɕϊ�����
				mTransformMatrix.M()[i] = atof(model->GetToken());
			}
			model->GetToken();
		}
		else if (strcmp(model->mToken, "Mesh") == 0)
		{
		mpMesh = new CMesh;
		mpMesh->Init(model);
		}
		else
		{
		//��L�ȊO�̗v�f�͓ǂݔ�΂�
		model->SlipNode();
		}
	}
	//�f�o�b�O�o�[�W�����̂ݗL��
#ifdef _DEBUG
	printf("%s\n", mpName);
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			printf("%f	", mTransformMatrix.M(i, j));
		}
		printf("\n");
	}
#endif
}

CModelXFrame::~CModelXFrame()
{
	if (mpMesh != nullptr)
		delete mpMesh;
}

/*
Render
���b�V�������݂���Ε`�悷��
*/
void CModelXFrame::Render()
{
	if (mpMesh != nullptr)
		mpMesh->Render();
}

//�R���X�g���N�^
CMesh::CMesh()
	: mFaceNum(0)
	, mVertexNum(0)
	, mNormalNum(0)
	, mpVertexIndex(nullptr)
	, mpVertex(nullptr)
	, mpNormal(nullptr)
{

}

//�f�X�g���N�^
CMesh::~CMesh()
{
	SAFE_DELETE_ARRA(mpVertex);
	SAFE_DELETE_ARRA(mpVertexIndex);
	SAFE_DELETE_ARRA(mpNormal);
}

/*
Init
Mesh�̃f�[�^��ǂݍ���
*/
void CMesh::Init(CModelX* model)
{
	model->GetToken();	//{ or ���O
	if (!strchr(model->mToken, '{'))
	{
		//���O���̏ꍇ�A����{
		model->GetToken();	// {
	}
	//���_���̎擾
	mVertexNum = atoi(model->GetToken());
	//���_�����G���A�m��
	mpVertex = new CVector[mVertexNum];
	//���_�����f�[�^����荞��
	for (int i = 0; i < mVertexNum; i++)
	{
		mpVertex[i].X(atof(model->GetToken()));
		mpVertex[i].Y(atof(model->GetToken()));
		mpVertex[i].Z(atof(model->GetToken()));
	}
	//�ʐ��̓ǂݍ���
	mFaceNum = atoi(model->GetToken());
	//���_����1�ʂ�3���_
	mpVertexIndex = new int[mFaceNum * 3];
	for (int i = 0; i < mFaceNum * 3; i += 3)
	{
		model->GetToken(); //���_���ǂݔ�΂�
		mpVertexIndex[i] = atoi(model->GetToken());
		mpVertexIndex[i + 1] = atoi(model->GetToken());
		mpVertexIndex[i + 2] = atoi(model->GetToken());
	}

	model->GetToken(); //MeshNormals
	if (strcmp(model->Token(), "MeshNormals") == 0)
	{
		model->GetToken(); // {
		//�@���f�[�^���擾
		mNormalNum = atoi(model->GetToken());
		//�@���f�[�^��z��Ɏ�荞��
		CVector* pNormal = new CVector[mNormalNum];
		for (int i = 0; i < mNormalNum; i++)
		{
			pNormal[i].X(atof(model->GetToken()));
			pNormal[i].Y(atof(model->GetToken()));
			pNormal[i].Z(atof(model->GetToken()));
		}
		//�@���� = �ʐ� �~�@3
		mNormalNum = atoi(model->GetToken()) * 3; //FaceNum
		int ni;
		//���_���ɖ@���f�[�^��ݒ肷��
		mpNormal = new CVector[mNormalNum];
		for (int i = 0; i < mNormalNum; i += 3)
		{
			model->GetToken(); //3
			ni = atoi(model->GetToken());
			mpNormal[i] = pNormal[ni];

			ni = atoi(model->GetToken());
			mpNormal[i + 1] = pNormal[ni];

			ni = atoi(model->GetToken());
			mpNormal[i + 2] = pNormal[ni];

		}
		delete[] pNormal;
		model->GetToken(); // }
	} //End of MeshNormal

	//�f�o�b�O�o�[�W�����̒ǉ�
#ifdef _DEBUG
	//���_�f�[�^�̏o��
	printf("VertexNum:%d\n", mVertexNum);
	for (int i = 0; i < mVertexNum; i++)
	{
	printf("%f	%f	%f	\n", mpVertex[i].X(), mpVertex[i].Y(), mpVertex[i].Z());
	}
	//�ʂ̃f�[�^�̏o��
	printf("FaceNum:%d\n", mFaceNum);
	for (int i = 0; i < mFaceNum * 3; i += 3)
	{
		printf("%d	%d	%d	\n",
			mpVertexIndex[i], mpVertexIndex[i + 1], mpVertexIndex[i + 2]);
	}
	//�@���f�[�^�̏o��
	printf("NormalNum:%d\n", mNormalNum);
	for (int i = 0; i < mNormalNum; i++)
	{
		printf("%f	%f	%f	\n",
			mpNormal[i].X(), mpNormal[i].Y(), mpNormal[i].Z());
	}
#endif
}

/*
Render
��ʂɕ`�悷��
*/
void CMesh::Render()
{
	/*���_�f�[�^,�@���f�[�^�̔z���T�q�ɂ���*/
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);

	/*���_�f�[�^,�@���f�[�^�̏ꏊ���w�肷��*/
	glVertexPointer(3, GL_FLOAT, 0, mpVertex);
	glNormalPointer(GL_FLOAT, 0, mpNormal);

	/*���_�̃C���f�b�N�X�̏ꏊ���w�肵�Đ}�`��`�悷��*/
	glDrawElements(GL_TRIANGLES, 3 * mFaceNum,GL_UNSIGNED_INT, mpVertexIndex);

	/*���_�f�[�^,�@���f�[�^�̔z��𖳌��ɂ���*/
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
}