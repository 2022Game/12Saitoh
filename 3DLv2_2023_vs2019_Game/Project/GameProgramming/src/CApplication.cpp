#include "CApplication.h"
#include "glut.h"
#include "CVector.h"
#include "CTriangle.h"
#include "CPlayer.h"
#include "CEnemy.h"
#include "CBillBoard.h"
#include "CEnemy3.h"
#include "CAlly.h"

#define MODEL_OBJ "res\\SnowGolem.obj","res\\SnowGolem.mtl"	//���f���f�[�^�̎w��
#define MODEL_BACKGROUND "res\\sky.obj", "res\\sky.mtl"		//�w�i���f���f�[�^�̎w��
#define MODEL_C5 "res\\c5.obj", "res\\c5.mtl"

CUi* CApplication::spUi = nullptr;

CUi* CApplication::Ui()
{
	return spUi; //�C���X�^���X�̃|�C���^��Ԃ�
}

CMatrix CApplication::mModelViewInverse;

const CMatrix& CApplication::ModelViewInverse()
{
	return mModelViewInverse;
}
//�N���X��static�ϐ�
CTexture CApplication::mTexture;

CTexture* CApplication::Texture()
{
	return &mTexture;
}

void CApplication::Start()
{
	spUi = new CUi(); //Ui�N���X�̐���
	//���f���t�@�C���̓���
	mModel.Load(MODEL_OBJ);
	//C5���f���̓ǂݍ���;
	mBackGround.Load(MODEL_BACKGROUND);
	mMatrix.Print();
	mPlayer.Model(&mModel);
	mPlayer.Scale(CVector(0.03f, 0.03f, 0.03f));
	mPlayer.Position(CVector(0.0f, -1.0f, -3.0f));
	mPlayer.Rotation(CVector(0.0f, 0.0f, 0.0f));
	mpMapManager = new CMapManager();

	new CEnemy3(CVector(-5.0f, -1.0f, -10.0f), CVector(), CVector(0.03f, 0.03f, 0.03f));
	new CEnemy3(CVector(5.0f, -1.0f, -10.0f), CVector(), CVector(0.03f, 0.03f, 0.03f));
	new CAlly(CVector(5.0f, -1.0f, 10.0f), CVector(), CVector(0.03f, 0.03f, 0.03f));
	new CAlly(CVector(-5.0f, -1.0f, 10.0f), CVector(), CVector(0.03f, 0.03f, 0.03f));

	//�r���{�[�h�̍쐬
	new CBillBoard(CVector(-6.0f, 3.0f, -10.0f), 1.0f, 1.0f);
	//�w�i���f������O�p�R���C�_�𐶐�
	// �e�C���X�^���X�Ɛe�s��͂�
	mColliderMesh.Set(nullptr, nullptr, &mBackGround);
}

void CApplication::Update()
{
	//�^�X�N�}�l�[�W���̍X�V
	CTaskManager::Instance()->Update();
	//�^�X�N�}�l�[�W���̏Փˏ���
	CTaskManager::Instance()->Collision();

	//�f�o�b�O�p(�J�����̐؂�ւ�)
	if (mInput.PullKey('C'))
	{
		if (CameraFlag == false)
		{
			CameraFlag = true;
		}
		else
		{
			CameraFlag = false;
		}
	}

	//���_1, ���_2, ���_3, �@���f�[�^�̍쐬
	CVector v0, v1, v2, n;
	//�@����������Őݒ肷��
	n.Set(0.0f, 1.0f, 0.0f);
	//���_1�̍��W��ݒ肷��
	v0.Set(0.0f, 0.0f, 0.5f);
	//���_2�̍��W��ݒ肷��
	v1.Set(1.0f, 0.0f, 0.0f);
	//���_3�̍��W��ݒ肷��
	v2.Set(0.0f, 0.0f, -0.5f);
	
	//�J�����̃p�����[�^���쐬����
	CVector e, c, u; //���_�A�����_�A�����
	if (CameraFlag == false)
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
	else
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

	//���f���r���[�s��̎擾
	glGetFloatv(GL_MODELVIEW_MATRIX, mModelViewInverse.M());
	//�t�s��̎擾
	mModelViewInverse = mModelViewInverse.Transpose();
	mModelViewInverse.M(0, 3, 0);
	mModelViewInverse.M(1, 3, 0);
	mModelViewInverse.M(2, 3, 0);
	mBackGround.Render();
	spUi->Render(); //Ui�̕`��
	//�^�X�N���X�g�̍폜
	CTaskManager::Instance()->Delete();
	//�^�X�N�}�l�[�W���̕`��
	CTaskManager::Instance()->Render();
	//�R���W�����}�l�[�W���`��
	CCollisionManager::Instance()->Render();
}

CApplication::~CApplication()
{
	delete spUi; //�C���X�^���XUi�̍폜
}