#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "CModelX.h"
#include "glut.h"

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

/*
CModelXFrame
model:CModelX�C���X�^���X�ւ̃|�C���^
�v���[�����쐬����
�ǂݍ��ݒ���Frame��������΁A�t���[�����쐬���A
�q�t���[���ɒǉ�����
*/
CModelXFrame::CModelXFrame(CModelX* model)
	: mpName(nullptr)
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
				mTransformMatrix.M()[i] = atof(model->GetToken());
			}
			model->GetToken();
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
	int i = 0;
	int j = 0;
	for (i = 0; i < 4; i++)
	{
		for (j = 0; j < 4; j++)
		{
			printf(" %f  ", mTransformMatrix.M(i, j));
		}
		printf("\n");
	}
#endif
}

CModelXFrame::~CModelXFrame()
{

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