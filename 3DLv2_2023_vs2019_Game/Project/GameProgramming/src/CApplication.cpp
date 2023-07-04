#include "CApplication.h"
#include "glut.h"
#include "CVector.h"
#include "CTriangle.h"
#include "CPlayer.h"
#include "CBillBoard.h"
#include "CEnemy3.h"
#include "CAlly.h"
#include <stdlib.h>
#include <time.h>

#define MODEL_OBJ "res\\SnowGolem.obj","res\\SnowGolem.mtl"	//���f���f�[�^�̎w��
#define MODEL_BACKGROUND "res\\Graund.obj", "res\\Graund.mtl"		//�w�i���f���f�[�^�̎w��
#define MODEL_C5 "res\\c5.obj", "res\\c5.mtl"

int CApplication::mCameraFlag = 0;
CUi* CApplication::spUi = nullptr;
CMatrix CApplication::mModelViewInverse;
CTexture CApplication::mTexture;

//�J�����̐ݒ�
void CApplication::Camera()
{
	//�f�o�b�O�p(�J�����̐؂�ւ�)
	if (mInput.PullKey('C'))
	{
		mCameraFlag++;
		if (mCameraFlag >= 3)
		{
			mCameraFlag = 0;
		}
	}
	//�J�����̃p�����[�^���쐬����
	CVector e, c, u; //���_�A�����_�A�����
	if (mCameraFlag == 0)
	{
		//���_�����߂�
		e = mPlayer.Position() + CVector(0.0f, 2.0f, 0.0f) * mPlayer.MatrixRotate();
		//�����_�����߂�
		c = mPlayer.Position() + CVector(0.0f, 2.0f, 1.0f) * mPlayer.MatrixRotate();
		//����������߂�
		u = CVector(0.0f, 0.1f, 0.0f) * mPlayer.MatrixRotate();
		//�J�����̐ݒ�
		gluLookAt(e.X(), e.Y(), e.Z(), c.X(), c.Y(), c.Z(), u.X(), u.Y(), u.Z());
	}
	else if (mCameraFlag == 1)
	{
		//���_�����߂�
		e = mPlayer.Position() + CVector(-0.9f, 3.0f, -12.0f) * mPlayer.MatrixRotate();
		//�����_�����߂�
		c = mPlayer.Position();
		//����������߂�
		u = CVector(0.0f, 1.0f, 0.0f) * mPlayer.MatrixRotate();
		//�J�����̐ݒ�
		gluLookAt(e.X(), e.Y(), e.Z(), c.X(), c.Y(), c.Z(), u.X(), u.Y(), u.Z());
	}
	else
	{
		//���_�����߂�
		e = mPlayer.Position() + CVector(0.5f, 3.0f, 12.0f) * mPlayer.MatrixRotate();
		//�����_�����߂�
		c = mPlayer.Position();
		//����������߂�
		u = CVector(0.0f, 1.0f, 0.0f) * mPlayer.MatrixRotate();
		//�J�����̐ݒ�
		gluLookAt(e.X(), e.Y(), e.Z(), c.X(), c.Y(), c.Z(), u.X(), u.Y(), u.Z());

	}
}

void CApplication::CreateModel()
{
	//���f���t�@�C���̓���
	mModel.Load(MODEL_OBJ);
	//�w�i���f���̓ǂݍ���;
	mBackGround.Load(MODEL_BACKGROUND);
	mMatrix.Print();
	mpMapManager = new CMapManager();
	mPlayer.Scale(CVector(0.03f, 0.03f, 0.03f));
	mPlayer.Position(CVector(0.0f, 0.0f, -3.0f));
	mPlayer.Rotation(CVector(0.0f, 0.0f, 0.0f));

	new CEnemy3(CVector(-5.0f, 0.0f, -10.0f), CVector(), CVector(0.03f, 0.03f, 0.03f));
	//new CEnemy3(CVector(5.0f, 0.0f, -10.0f), CVector(), CVector(0.03f, 0.03f, 0.03f));
	//new CAlly(CVector(5.0f, 0.0f, 10.0f), CVector(), CVector(0.03f, 0.03f, 0.03f));
	//new CAlly(CVector(-5.0f, 0.0f, 10.0f), CVector(), CVector(0.03f, 0.03f, 0.03f));

	//�r���{�[�h�̍쐬
	//new CBillBoard(CVector(-6.0f, 3.0f, -10.0f), 1.0f, 1.0f);
	//�w�i���f������O�p�R���C�_�𐶐�
	// �e�C���X�^���X�Ɛe�s��͂�
	mColliderMesh.Set(nullptr, nullptr, &mBackGround);
}

CApplication::~CApplication()
{
	delete spUi; //�C���X�^���XUi�̍폜
}

CUi* CApplication::Ui()
{
	return spUi; //�C���X�^���X�̃|�C���^��Ԃ�
}

const CMatrix& CApplication::ModelViewInverse()
{
	return mModelViewInverse;
}

CTexture* CApplication::Texture()
{
	return &mTexture;
}

int CApplication::CameraFlag()
{
	return mCameraFlag;
}

void CApplication::Start()
{
	//�����_���l�̎擾���@
	srand((unsigned int)time(NULL));
	for (int i = 0; i < 10; i++)
	{
		//�ŏ��l0 ,�擾�͈�3 
		printf("%d\n", 0 + rand() % 3);
	}
	spUi = new CUi(); //Ui�N���X�̐���
	mState = EState::ESTART;
}

void CApplication::Update()
{

	switch (mState)
	{
	case EState::ESTART:
		mPlayer.Model(&mModel);
		spUi->Start();
		//�^�X�N�}�l�[�W���̍X�V
		CTaskManager::Instance()->Update();
		//�^�X�N�}�l�[�W���̕`��
		CTaskManager::Instance()->Render();
		if (mInput.PushKey(VK_RETURN))
		{
			mState = EState::EPLAY;
			//���f���𐶐�
			CApplication::CreateModel();
		}
		break;
	case EState::EPLAY:
		CTaskManager::Instance()->Update();
		CApplication::Camera();
		if (CPlayer::HP() == 0)
		{
			mState = EState::EOVER;
		}
		//���f���r���[�s��̎擾
		glGetFloatv(GL_MODELVIEW_MATRIX, mModelViewInverse.M());
		//�t�s��̎擾
		mModelViewInverse = mModelViewInverse.Transpose();
		mModelViewInverse.M(0, 3, 0);
		mModelViewInverse.M(1, 3, 0);
		mModelViewInverse.M(2, 3, 0);
		mBackGround.Render();
		//�^�X�N���X�g�̍폜
		CTaskManager::Instance()->Delete();
		//�^�X�N�}�l�[�W���̏Փˏ���
		CTaskManager::Instance()->Collision();
		//�^�X�N�}�l�[�W���̕`��
		CTaskManager::Instance()->Render();
		//�R���W�����}�l�[�W���`��
		CCollisionManager::Instance()->Render();
		break;
	case EState::ECLEAR:
		break;
	case EState::EOVER:
		spUi->Over();
		CTaskManager::Instance()->Update();
		//�^�X�N�}�l�[�W���̕`��
		CTaskManager::Instance()->Render();
		break;
	}
}