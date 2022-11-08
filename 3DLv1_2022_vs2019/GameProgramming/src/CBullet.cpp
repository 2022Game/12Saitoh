#include"CBullet.h"

//���Ɖ��s���̐ݒ�
//Set(��,���s)
void CBullet::Set(float w, float d)
{
	//�X�P�[���ݒ�
	mScale = CVector(1.0f, 1.0f, 1.0f);
	//�O�p�`�̒��_�ݒ�
	mT.Vertex(CVector(w, 0.0f, 0.0f)*mMatrixRotate, CVector(0.0f, 0.0f, -d)*mMatrixRotate, CVector(-w, 0.0f, 0.0f)*mMatrixRotate);
	//�O�p�`�̖@���ݒ�
	mT.Normal(CVector(0.0f, 1.0f, 0.0f));
}

//�X�V
void CBullet::Update()
{
	CTransform::Update();
	//�ʒu�X�V
	mPosition = mPosition + CVector(0.0f,0.0f,1.0f) * mMatrixRotate;
}

//�`��
void CBullet::Render()
{
	//DIFFUSE���F�ݒ�
	float c[] = { 1.0f, 1.0f, 0.0f, 1.0f };
	glMaterialfv(GL_FRONT, GL_DIFFUSE, c);
	//�O�p�`�`��
	mT.Render(CMatrix().Translate(mPosition.X(), mPosition.Y(), mPosition.Z()));
}