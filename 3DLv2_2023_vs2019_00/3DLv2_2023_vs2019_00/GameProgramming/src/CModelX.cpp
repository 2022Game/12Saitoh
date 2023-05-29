#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "CModelX.h"
#include "glut.h"
#include "CMaterial.h"

//�z��̃T�C�Y�擾���}�N����
#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))

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

std::vector<CModelXFrame*>& CModelX::Frames()
{
	return mFrame;
}

std::vector<CAnimationSet*>& CModelX::AnimationSet()
{
	return mAnimationSet;
}

/*
FinedFrame(�t���[����)
�t���[�����ɊY������t���[���̃A�h���X��Ԃ�
*/
CModelXFrame* CModelX::FinedFrame(char* name)
{
	//�C�e���[�^�̍쐬
	std:: vector<CModelXFrame*>::iterator itr;
	//�擪����Ō�܂ŌJ��Ԃ�
	for (itr = mFrame.begin(); itr != mFrame.end(); itr++)
	{
		//���O����v�������H
		if (strcmp(name, (*itr)->mpName) == 0)
		{
			//��v�����炻�̃A�h���X��Ԃ�
			return *itr;
		}
	}
	//��v����t���[���������ꍇ��nullptr��Ԃ�
	return nullptr;
}

CModelX::CModelX()
	: mpPointer(nullptr)
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
	for (size_t i = 0; i < mAnimationSet.size(); i++)
	{
		delete mAnimationSet[i];
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

bool CModelX::EOT()
{
	if (*mpPointer == '\0')
	{
		return true;
	}
	else
	{
		return false;
	}
}

/*
AnimateVertex
���_�ɃA�j���[�V������K�p����
*/
void CModelX::AnimateVertex()
{
	//�t���[�������J��Ԃ�
	for (size_t i = 0; i < mFrame.size(); i++)
	{
		//���b�V���ɖʂ������
		if (mFrame[i]->mpMesh != nullptr)
		{
			//���_���A�j���[�V�����ōX�V����
			mFrame[i]->mpMesh->AnimateVertex(this);
		}
	}
}

void CModelX::SetSkinWeightFrameIndex()
{
	//�t���[�������J��Ԃ�
	for (size_t i = 0; i < mFrame.size(); i++)
	{
		//���b�V�����L���
		if (mFrame[i]->mpMesh != nullptr)
		{
			mFrame[i]->mpMesh->SetSkinWeightFrameIndex(this);
		}
	}
}

/*
AnimateFrame
�t���[���̕ϊ��s����A�j���[�V�����f�[�^�ōX�V����
*/
void CModelX::AnimateFrame()
{
	//�A�j���[�V�����œK�p�����t���[���̕ϊ��s����[���N���A����
	for (size_t i = 0; i < mAnimationSet.size(); i++)
	{
		CAnimationSet* animSet = mAnimationSet[i];
		//�d�݂�0����΂�
		if (animSet->mWeight == 0) continue;
		//�t���[����(Animation��)�J��Ԃ�
		for (size_t j = 0; j < animSet->Animation().size(); j++)
		{
			CAnimation* animation = animSet->Animation()[j];
			//�Y������t���[���̕ϊ��s����[���N���A����
			memset(&mFrame[animation->mFrameIndex]->mTransformMatrix, 0, sizeof(CMatrix));
		}
	}
	//�A�j���[�V�����ɊY������t���[���̕ϊ��s����A�j���[�V�����̃f�[�^�Őݒ肷��
	for (size_t i = 0; i < mAnimationSet.size(); i++)
	{
		CAnimationSet* animSet = mAnimationSet[i];
		//�d�݂�0�͔�΂�
		if (animSet->mWeight == 0) continue;
		animSet->AnimateMarix(this);
	}
#ifdef _DEBUG
	//for (int i = 1; i < 5; i++)
	//{
	//	printf("Frame:%s\n", mFrame[i]->mpName);
	//	for (int row = 0; row < 4; row++)
	//	{
	//		for (int col = 0; col < 4; col++)
	//		{
	//			printf("%f	", mFrame[i]->mTransformMatrix.M(row, col));
	//		}
	//		printf("\n");
	//	}
	//}
#endif
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
		//�P�ꂪAnimationSet�̏ꍇ
		else if (strcmp(mToken, "AnimationSet") == 0)
		{
			new CAnimationSet(this);
		}
	}
	fclose(fp); //�t�@�C�����N���[�Y����

	SAFE_DELETE_ARRA(buf); //�m�ۂ����̈���������
	//�X�L���E�F�C�g�̃t���[���ԍ��ݒ�
	SetSkinWeightFrameIndex();
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
	//printf("%s\n", mpName);
	//for (int i = 0; i < 4; i++)
	//{
	//	for (int j = 0; j < 4; j++)
	//	{
	//		printf("%f	", mTransformMatrix.M(i, j));
	//	}
	//	printf("\n");
	//}
#endif
}

CModelXFrame::~CModelXFrame()
{
	//�q�t���[����S�ĉ������
	std::vector<CModelXFrame*>::iterator itr;
	for (itr = mChild.begin(); itr != mChild.end(); itr++) {
		delete* itr;
	}
	//���O�̃G���A���������
	SAFE_DELETE_ARRA(mpName);

	if (mpMesh != nullptr)
		delete mpMesh;
}

int CModelXFrame::Index()
{
	return mIndex;
}

const CMatrix& CModelXFrame::CombinedMatrix()
{
	return mCombinedMatrix;
}

/*
AnimateCombined
�����s��̍쐬
*/
void CModelXFrame::AnimateCombined(CMatrix* parent)
{
	//�����̕ϊ��s��ɁA�e����ϊ��s����|����
	mCombinedMatrix = mTransformMatrix * (*parent);
	//�q�t���[���̍����s����쐬����
	for (size_t i = 0; i < mChild.size(); i++)
	{
		mChild[i]->AnimateCombined(&mCombinedMatrix);
	}
#ifdef _DEBUG
	//printf("Frame:%s\n", mpName);
	//for (int i = 0; i < 4; i++)
	//{
	//	for (int j = 0; j < 4; j++)
	//	{
	//		printf("%f	", mCombinedMatrix.M(i, j));
	//	}
	//	printf("\n");
	//}
#endif
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
	, mMaterialNum(0)
	, mMaterialIndexNum(0)
	, mpMaterialIndex(nullptr)
	, mpVertexIndex(nullptr)
	, mpVertex(nullptr)
	, mpAnimateVertex(nullptr)
	, mpAnimateNormal(nullptr)
	, mpNormal(nullptr)
{

}

//�f�X�g���N�^
CMesh::~CMesh()
{
	SAFE_DELETE_ARRA(mpVertex);
	SAFE_DELETE_ARRA(mpVertexIndex);
	SAFE_DELETE_ARRA(mpNormal);
	SAFE_DELETE_ARRA(mpMaterialIndex);
	SAFE_DELETE_ARRA(mpAnimateVertex);
	SAFE_DELETE_ARRA(mpAnimateNormal);
	//�X�L���E�F�C�g�̍폜
	for (size_t i = 0; i < mSkinWeights.size(); i++)
	{
		delete mSkinWeights[i];
	}
}

void CMesh::AnimateVertex(CModelX* model)
{
	//�A�j���[�V�����p�̒��_�G���A�N���A
	memset(mpAnimateVertex, 0, sizeof(CVector) * mVertexNum);
	memset(mpAnimateNormal, 0, sizeof(CVector) * mNormalNum);
	//�X�L���E�F�C�g���J��Ԃ�
	for (size_t i = 0; i < mSkinWeights.size(); i++)
	{
		//�t���[���ԍ��擾
		int frameIndex = mSkinWeights[i]->mFrameIndex;
		//�I�t�Z�b�g�s��ƃt���[�������s�������
		CMatrix mSkinningMatrix = mSkinWeights[i]->mOffset * model->Frames()[frameIndex]->CombinedMatrix();
		//���_�����J��Ԃ�
		for (int j = 0; j < mSkinWeights[i]->mIndexNum; j++)
		{
			//���_�ԍ��擾
			int index = mSkinWeights[i]->mpIndex[j];
			//�d�ݎ擾
			float weight = mSkinWeights[i]->mpWeight[j];
			//���_�Ɩ@�����X�V����
			mpAnimateVertex[index] += mpVertex[index] * mSkinningMatrix * weight;
			mpAnimateNormal[index] += mpNormal[index] * mSkinningMatrix * weight;
		}
	}
	//�@���𐳋K������
	for (int i = 0; i < mNormalNum; i++)
	{
		mpAnimateNormal[i] = mpAnimateNormal[i].Normalize();
	}
}

void CMesh::SetSkinWeightFrameIndex(CModelX* model)
{
	//�X�L���E�F�C�g���J��Ԃ�
	for (size_t i = 0; i < mSkinWeights.size(); i++)
	{
		//�t���[�����̃t���[�����擾����
		CModelXFrame* frame = model->FinedFrame(mSkinWeights[i]->mpFrameName);
		//�t���[���ԍ���ݒ肷��
		mSkinWeights[i]->mFrameIndex = frame->Index();
	}
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
	mpAnimateVertex = new CVector[mVertexNum];
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

	//�P�ꂪ����ԌJ��Ԃ�
	while (!model->EOT())
	{
		model->GetToken(); //MeshNormals
		// }�������̏ꍇ�͏I��
		if (strchr(model->Token(), '}'))
			break;
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
			mpAnimateNormal = new CVector[mNormalNum];
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

		//MeshMaterialList�̂Ƃ�
		else if (strcmp(model->Token(), "MeshMaterialList") == 0)
		{
			model->GetToken();  // {
			//Material�̐�
			mMaterialNum = atoi(model->GetToken());
			//FaceNum
			mMaterialIndexNum = atoi(model->GetToken());
			//�}�e���A���C���f�b�N�X�̍쐬
			mpMaterialIndex = new int[mMaterialIndexNum];
			for (int i = 0; i < mMaterialIndexNum; i++)
			{
				mpMaterialIndex[i] = atoi(model->GetToken());
			}
			//�}�e���A���f�[�^�̍쐬
			for (int i = 0; i < mMaterialNum; i++)
			{
				model->GetToken();  //Material
				if (strcmp(model->Token(), "Material") == 0)
				{
					mMaterial.push_back(new CMaterial(model));
				}
			}
			model->GetToken();  // } End of MeshMaterialList
		}// End of MaterialList
		//SkinWeghts�̂Ƃ�
		else if (strcmp(model->Token(), "SkinWeights") == 0)
		{
			//CSkinWeghts�N���X�̃C���X�^���X���쐬���A�z��ɒǉ�
			mSkinWeights.push_back(new CSkinWeights(model));
		}
		else
		{
			//�ȊO�̃m�[�h�͓ǂݔ�΂�
			model->SlipNode();
		}
	}
#ifdef _DEBUG
	////���_�f�[�^�̏o��
	//printf("VertexNum:%d\n", mVertexNum);
	//for (int i = 0; i < mVertexNum; i++)
	//{
	//printf("%f	%f	%f	\n", mpVertex[i].X(), mpVertex[i].Y(), mpVertex[i].Z());
	//}
	////�ʂ̃f�[�^�̏o��
	//printf("FaceNum:%d\n", mFaceNum);
	//for (int i = 0; i < mFaceNum * 3; i += 3)
	//{
	//	printf("%d	%d	%d	\n",
	//		mpVertexIndex[i], mpVertexIndex[i + 1], mpVertexIndex[i + 2]);
	//}
	////�@���f�[�^�̏o��
	//printf("NormalNum:%d\n", mNormalNum);
	//for (int i = 0; i < mNormalNum; i++)
	//{
	//	printf("%f	%f	%f	\n",
	//		mpNormal[i].X(), mpNormal[i].Y(), mpNormal[i].Z());
	//}
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
	glVertexPointer(3, GL_FLOAT, 0, mpAnimateVertex);
	glNormalPointer(GL_FLOAT, 0, mpAnimateNormal);

	/*���_�C���f�b�N�X�̏ꏊ���w�肵�Đ}�`��`�悷��*/
	for (int i = 0; i < mFaceNum; i++)
	{
		//�}�e���A����K�p����
		mMaterial[mpMaterialIndex[i]]->Enabled();
		/*���_�̃C���f�b�N�X�̏ꏊ���w�肵�Đ}�`��`�悷��*/
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (mpVertexIndex + i * 3));
	}

	/*���_�f�[�^,�@���f�[�^�̔z��𖳌��ɂ���*/
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
}

/*
CSkinWeights
�X�L���E�F�C�g�̓ǂݍ���
*/
CSkinWeights::CSkinWeights(CModelX* model)
	: mpFrameName(0)
	, mFrameIndex(0)
	, mIndexNum(0)
	, mpIndex(nullptr)
	, mpWeight(nullptr)
{
	model->GetToken();	// {
	model->GetToken();	// Framename
	//�t���[�����G���A�m�ہA�ݒ�
	mpFrameName = new char[strlen(model->Token()) + 1];
	strcpy(mpFrameName, model->Token());

	//���_�ԍ����擾
	mIndexNum = atoi(model->GetToken());
	//���_�ԍ���0�𒴂���
	if (mIndexNum > 0)
	{
		//���_�ԍ��ƒ��_�E�F�C�g�̃G���A�m��
		mpIndex = new int[mIndexNum];

		mpWeight = new float[mIndexNum];
		//���_�ԍ��擾
		for (int i = 0; i < mIndexNum; i++)
			mpIndex[i] = atoi(model->GetToken());
		//���_�E�F�C�g�擾
		for (int i = 0; i < mIndexNum; i++)
			mpWeight[i] = atof(model->GetToken());
	}
	//�I�t�Z�b�g�s��擾
	for (int i = 0; i < 16; i++)
	{
		mOffset.M()[i] = atof(model->GetToken());
	}
	model->GetToken();	// }
#ifdef _DEBUG
	////SkinWeghts�@�t���[�����̏o��
	//printf("SKinWeights %s\n",mpFrameName);
	////���_�ԍ��@���_�E�F�C�g�̏o��
	//for (int i = 0; i < mIndexNum; i++)
	//{
	//	printf("%d  %f\n",mpIndex[i],mpWeight[i]);
	//}
	////�I�t�Z�b�g�s��̏o��
	//for (int i = 0; i < 4; i++)
	//{
	//	for (int j = 0; j < 4; j++)
	//	{
	//		printf("%f	", mOffset.M(i,j));
	//	}
	//	printf("\n");
	//}
#endif
}

CSkinWeights::~CSkinWeights()
{
	SAFE_DELETE_ARRA(mpFrameName);
	SAFE_DELETE_ARRA(mpIndex);
	SAFE_DELETE_ARRA(mpWeight);
}

/*
CAnimationSet
*/
CAnimationSet::CAnimationSet(CModelX* model)
	: mpName(nullptr)
	, mTime(0)
	, mWeight(0)
	, mMaxTime(0)
{
	model->mAnimationSet.push_back(this);
	model->GetToken();	//Animation Name
	//�A�j���[�V�����Z�b�g����ޔ�
	mpName = new char[strlen(model->Token()) + 1];
	strcpy(mpName, model->Token());
	model->GetToken();	// {
	while (!model->EOT())
	{
		model->GetToken();	// } or Animation
		if (strchr(model->Token(), '}'))
			break;
		if (strcmp(model->Token(), "Animation") == 0)
		{
			//Animation�v�f�̓ǂݍ���
			mAnimation.push_back(new CAnimation(model));
		}
	}
	//�I�����Ԑݒ�
	mMaxTime = mAnimation[0]->mpKey[mAnimation[0]->mKeyNum - 1].mTime;
#ifdef _DEBUG
	//printf("AnimationSet:%s\n", mpName);
#endif
}

CAnimationSet::~CAnimationSet()
{
	//�A�j���[�V�����v�f�̍폜
	for (size_t i = 0; i < mAnimation.size(); i++)
	{
		delete mAnimation[i];
	}
}


float CAnimationSet::Time()
{
	return mTime;
}

float CAnimationSet::MaxTime()
{
	return mMaxTime;
}

std::vector<CAnimation*>& CAnimationSet::Animation()
{
	return mAnimation;
}

void CAnimationSet::AnimateMarix(CModelX* model)
{
	//�d�݂�0�͔�΂�
	if (mWeight == 0) return;
	//�t���[����(Animation��)�J��Ԃ�
	for (size_t j = 0; j < mAnimation.size(); j++)
	{
		//�t���[�����擾����
		CAnimation* animation = mAnimation[j];
		//�L�[�������ꍇ�͎��̃A�j���[�V������
		if (animation->mpKey == nullptr) continue;
		//�Y������t���[���̎擾
		CModelXFrame* frame = model->mFrame[animation->mFrameIndex];
		//�ŏ��̎��Ԃ�菬�����ꍇ
		if (mTime < animation->mpKey[0].mTime)
		{
			//�ϊ��s���0�R�}�ڂ̍s��ōX�V
			frame->mTransformMatrix += animation->mpKey[0].mMatrix * mWeight;
		}
		//�Ō�̎��Ԃ��傫���ꍇ
		else if (mTime >= animation->mpKey[animation->mKeyNum - 1].mTime)
		{
			//�ϊ��s����Ō�̃R�}�̍s��ōX�V
			frame->mTransformMatrix += animation->mpKey[animation->mKeyNum - 1].mMatrix * mWeight;
		}
		else
		{
			//���Ԃ̓r���̏ꍇ
			for (int k = 1; k < animation->mKeyNum; k++)
			{
				//�ϊ��s����A���`��ԂɂčX�V
				if (mTime < animation->mpKey[k].mTime &&
					animation->mpKey[k - 1].mTime != animation->mpKey[k].mTime)
				{
					float r = (animation->mpKey[k].mTime - mTime) /
						(animation->mpKey[k].mTime - animation->mpKey[k - 1].mTime);

					frame->mTransformMatrix +=
						(animation->mpKey[k - 1].mMatrix * r +
							animation->mpKey[k].mMatrix * (1 - r)) * mWeight;
					break;
				}
			}
		}
	}
}

void CAnimationSet::Time(float time)
{
	mTime = time;
}

void CAnimationSet::Weight(float weight)
{
	mWeight = weight;
}

/*
CAnimation
�A�j���[�V�����N���X
*/
CAnimation::CAnimation(CModelX* model)
	: mpFrameName(nullptr)
	, mFrameIndex(0)
	, mKeyNum(0)
	, mpKey(nullptr)
{
	model->GetToken(); // { or Animation Name
	if (strchr(model->Token(), '{'))
	{
		model->GetToken(); // {
	}
	else
	{
		model->GetToken(); // {
		model->GetToken(); // {
	}
	model->GetToken(); //FrameName
	mpFrameName = new char[strlen(model->Token()) + 1];
	strcpy(mpFrameName, model->Token());
	mFrameIndex = 
		model->FinedFrame(model->Token())->Index();
	model->GetToken();	// }
	//�L�[�̔z���ۑ����Ă����z��
	CMatrix* key[4] = { 0,0,0,0 };
	//���Ԃ̔z���ۑ����Ă����z��
	float* time[4] = { 0,0,0,0 };
	while (!model->EOT())
	{
		model->GetToken();	// { or AnimationKey
		if (strchr(model->Token(), '}'))
			break;
		if (strcmp(model->Token(), "AnimationKey") == 0)
		{
			model->GetToken();	// {
			//�f�[�^�̃^�C�v�擾
			int type = atoi(model->GetToken());
			//���Ԑ��擾
			mKeyNum = atoi(model->GetToken());
			switch (type)
			{
			case 0: // Rotation Quaternion
				//�s��̔z������Ԑ����m��
				key[type] = new CMatrix[mKeyNum];
				//���Ԃ̔z������Ԑ����m��
				time[type] = new float[mKeyNum];
				//���Ԑ����J��Ԃ�
				for(int i = 0; i < mKeyNum; i++)
				{
					//���Ԏ擾
					time[type][i] = atof(model->GetToken());
					model->GetToken();	//4��ǂݔ�΂�
					//w,x,y,z���擾
					float w = atof(model->GetToken());
					float x = atof(model->GetToken());
					float y = atof(model->GetToken());
					float z = atof(model->GetToken());
					//�N�I�[�^�j�I�������]�s��ɕϊ�
					key[type][i].Quaternion(x, y, z, w);
				}
				break;
			case 1:	//�g��E�k���̍s��쐬
				key[type] = new CMatrix[mKeyNum];
				time[type] = new float[mKeyNum];
				for (int i = 0; i < mKeyNum; i++)
				{
					time[type][i] = atof(model->GetToken());
					model->GetToken();	//3
					float x = atof(model->GetToken());
					float y = atof(model->GetToken());
					float z = atof(model->GetToken());
					key[type][i].Scale(x, y, z);
				}
				break;
			case 2:	//�ړ��̍s��쐬
				key[type] = new CMatrix[mKeyNum];
				time[type] = new float[mKeyNum];
				for (int i = 0; i < mKeyNum; i++)
				{
					time[type][i] = atof(model->GetToken());
					model->GetToken();	//3
					float x = atof(model->GetToken());
					float y = atof(model->GetToken());
					float z = atof(model->GetToken());
					key[type][i].Translate(x, y, z);
				}
				break;
			case 4:	//�s��f�[�^���擾
				mpKey = new CAnimationKey[mKeyNum];
				for (int i = 0; i < mKeyNum; i++)
				{
					mpKey[i].mTime = atof(model->GetToken());	//Time
					model->GetToken();	//16
					for (int j = 0; j < 16; j++)
					{
						mpKey[i].mMatrix.M()[j] = atof(model->GetToken());
					}
				}
				break;
			}
			model->GetToken();	// }
		}
		else
		{
			model->SlipNode();
		}
	}
	//�s��̃f�[�^�ł͂Ȃ���
	if (mpKey == 0)
	{
		//���Ԑ����L�[���쐬
		mpKey = new CAnimationKey[mKeyNum];
		for (int i = 0; i < mKeyNum; i++)
		{
			//���Ԑݒ�
			mpKey[i].mTime = time[2][i];	//Time
			//�s��쐬 Size * Rotation * Position
			mpKey[i].mMatrix = key[1][i] * key[0][i] * key[2][i];
		}
	}
	//�m�ۂ����G���A���
	for (int i = 0; i < ARRAY_SIZE(key);i++)
	{
		SAFE_DELETE_ARRA(time[i]);
		SAFE_DELETE_ARRA(key[i]);
	}
#ifdef _DEBUG
	//printf("Animation::%s\n", mpFrameName);
	//for (int i = 0; i < 4; i++)
	//{
	//	for (int j = 0; j < 4; j++)
	//	{
	//		printf("%f	",mpKey[0].mMatrix.M(i,j));
	//	}
	//	printf("\n");
	//}
#endif
}

CAnimation::~CAnimation()
{
	SAFE_DELETE_ARRA(mpFrameName);
	SAFE_DELETE_ARRA(mpKey);
}