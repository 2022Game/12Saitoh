#include "CApplication.h"
#include "glut.h"
#include "CRectangle.h"
#include "CVector.h"
#include "CTriangle.h"
#include "CPlayer.h"

#define SOUND_BGM "res\\mario.wav" //BGM�����t�@�C��
#define SOUND_OVER "res\\mdai.wav" //�Q�[���I�[�o�[�����t�@�C��
#define MODEL_OBJ "res\\f14.obj","res\\f14.mtl"//���f���f�[�^�̎w��
#define MODEL_BACKGROUND "res\\sky.obj", "res\\sky.mtl"

CTaskManager CApplication::mTaskManager;
CTaskManager* CApplication::TaskManager()
{
	return &mTaskManager;
}
//�N���X��static�ϐ�
CTexture CApplication::mTexture;
CCharacterManager CApplication::mCharacterManager;

CCharacterManager* CApplication::CharacterManager()
{
	return &mCharacterManager;
}

CTexture* CApplication::Texture()
{
	return &mTexture;
}

void CApplication::Start()
{
	mEye = CVector(1.0f, 2.0f, 3.0f);
	//���f���t�@�C���̓���
	mModel.Load(MODEL_OBJ);
	mBackGround.Load(MODEL_BACKGROUND);
	matrix.Print();
	mPlayer.Model(&mModel);
	mPlayer.Scale(CVector(0.1f, 0.1f, 0.1f));
	mPlayer.Position(CVector(0.0f, 0.0f, -3.0f));
	mPlayer.Rotation(CVector(0.0f, 180.0f, 0.0f));
}

void CApplication::Update()
{
	if (mInput.Key('J'))
	{
		mEye = mEye - CVector(0.1f, 0.0f, 0.0f);
	}
	if (mInput.Key('L'))
	{
		mEye = mEye + CVector(0.1f, 0.0f, 0.0f);
	}
	if (mInput.Key('I'))
	{
		mEye = mEye - CVector(0.0f, 0.0f, 0.1f);
	}
	if (mInput.Key('K'))
	{
		mEye = mEye + CVector(0.0f, 0.0f, 0.1f);
	}
	if (mInput.Key('O'))
	{
		mEye = mEye + CVector(0.0f, 0.1f, 0.0f);
	}
	if (mInput.Key('M'))
	{
		mEye = mEye - CVector(0.0f, 0.1f, 0.0f);
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
	
	mPlayer.Update();
	//�J�����̃p�����[�^���쐬����
	CVector e, c, u; //���_�A�����_�A�����
	//���_�����߂�
	e = mPlayer.Position() + CVector(0.0f, 1.0f, -3.0f) * mPlayer.MatrixRotate();
	//�����_�����߂�
	c = mPlayer.Position();
	//����������߂�
	u = CVector(0.0f, 1.0f, 0.0f) * mPlayer.MatrixRotate();
	//�J�����̐ݒ�
	gluLookAt(e.X(), e.Y(), e.Z(), c.X(), c.Y(), c.Z(), u.X(), u.Y(), u.Z());
	mPlayer.Render();
	mBackGround.Render();
	//�^�X�N�}�l�[�W���̍X�V
	mTaskManager.Update();
	//�^�X�N�}�l�[�W���̕`��
	mTaskManager.Render();
	//CTransform trans; //�ϊ��s��C���X�^���X�̍쐬
	//trans.Position(CVector(0.5f, 1.8f, 0.5f)); //�ʒu�̐ݒ�
	//trans.Rotation(CVector(-10.0f, -20.0f, -30.0f)); //��]�̐ݒ�
	//trans.Scale(CVector(0.1f, 0.1f, 0.1f)); //�g��k���̐ݒ�
	//trans.Update(); //�s��̍X�V
	//mModel.Render(trans.Matrix());

	//CMatrix matrix, position, rotation, scale;
	//position.Translate(0.5f, 1.8f, 0.5f); //�ړ��s��ݒ�
	//rotation.RotateY(180.0f); //��]�s��ݒ�
	//scale.Scale(0.1f, 0.1f, 0.1f); //�g��k�����s��ݒ�
	//matrix = scale * rotation * position; //�����s��ݒ�
	//mModel.Render(matrix); //���f���̕`��
	////�`��J�n
	//glBegin(GL_TRIANGLES);

	////�@��(�ʂ̌���)�̐ݒ�
	//glNormal3f(n.X(), n.Y(), n.Z());

	////���_���W�̐ݒ�(�O�p�`1�̕`��)
	//glVertex3f(v0.X(), v0.Y(), v0.Z());
	//glVertex3f(v1.X(), v1.Y(), v1.Z());
	//glVertex3f(v2.X(), v2.Y(), v2.Z());

	////�@���ƒ��_�̐ݒ�
	//n.Set(0.0f, 0.0f, 1.0f);
	//v0.Set(0.5f, 0.0f, 0.0f);
	//v1.Set(0.0f, 1.0f, 0.0f);
	//v2.Set(-0.5f, 0.0f, 0.0f);
	////�O�p�`2�̕`��
	//glNormal3f(n.X(), n.Y(), n.Z());
	//glVertex3f(v0.X(), v0.Y(), v0.Z());
	//glVertex3f(v1.X(), v1.Y(), v1.Z());
	//glVertex3f(v2.X(), v2.Y(), v2.Z());

	////�@���ƒ��_�̐ݒ�
	//n.Set(1.0f, 0.0f, 0.0f);
	//v0.Set(0.0f, 0.5f, 0.0f);
	//v1.Set(0.0f, 0.0f, 1.0f);
	//v2.Set(0.0f, -0.5f, 0.0f);
	////�O�p�`3�̕`��
	//glNormal3f(n.X(), n.Y(), n.Z());
	//glVertex3f(v0.X(), v0.Y(), v0.Z());
	//glVertex3f(v1.X(), v1.Y(), v1.Z());
	//glVertex3f(v2.X(), v2.Y(), v2.Z());

	////�`�ʏI��
	//glEnd();

	////�O�p�`�N���X�̃C���X�^���X�쐬
	//CTriangle t0;
	////�@���ƒ��_�̐ݒ�
	//t0.Vertex(CVector(1.0f, 0.0f, 0.5f), CVector(2.0f, 0.0f, 0.0f), CVector(1.0f, 0.0f, -0.5f));
	//t0.Normal(CVector(0.0f, 1.0f, 0.0f));
	////�O�p�`�̕`��
	//t0.Render();

	//CTriangle t1;
	//t1.Vertex(CVector(0.5f, 1.0f, 0.0f), CVector(0.0f, 2.0f, 0.0f), CVector(-0.5f, 1.0f, 0.0f));
	//t1.Normal(CVector(0.0f, 0.0f, 1.0f));
	//t1.Render();

	//CTriangle t2;
	//t2.Vertex(CVector(0.0f, 0.5f, 1.0f), CVector(0.0f, 0.0f, 2.0f), CVector(0.0f, -0.5f, 1.0f));
	//t2.Normal(CVector(1.0f, 0.0f, 0.0f));
	//t2.Render();
}
