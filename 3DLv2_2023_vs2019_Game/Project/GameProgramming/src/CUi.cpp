#include <stdio.h>
#include "CUi.h"
#include "CCamera.h"
CUi::CUi()
	: mHp(0)
	, mTime(0)
	, mEnemy(0)
	, mPosY(0.0f)
	, mRotX(0.0f)
{
	mFont.Load("FontWhite.png", 1, 64);
}

void CUi::Clear()
{
	mFont.Draw(36, 300, 18, 36, "GAME CLEAR!!");
	mFont.Draw(36, 200, 18, 36, "PUSH ENTER KEY!");
}

void CUi::Over()
{
	CCamera::Start(0, 800, 0, 600); //2D�`��J�n
	mFont.Draw(250, 300, 18, 36, "GAME OVER!!");
	CCamera::End();  //2D�`��I��
}

void CUi::Start()
{
	CCamera::Start(0, 800, 0, 600); //2D�`��J�n
	mFont.Draw(36, 300, 18, 36, "START PUSH ENTER KEY!");
	CCamera::End();  //2D�`��I��
}

void CUi::Enemy(int enemy)
{
	mEnemy = enemy;
}

void CUi::Hp(int hp)
{
	mHp = hp;
}

void CUi::Time(int time)
{
	mTime = time;
}

void CUi::Render()
{
	CCamera::Start(0, 800, 0, 600); //2D�`��J�n
	//�`��F�̐ݒ�(�ΐF�̔�����)
	glColor4f(0.0f, 1.0f, 0.0f, 0.5f);
	//������ҏW�G���A�̍쐬
	char buf[64];

	mFont.Draw(30, 550, 8, 16, "C : CHANGE CAMERA");
	//Y���W�̕\��
	//������̐ݒ�
	sprintf(buf, "PY:%7.2f", mPosY);
	//�����̕`��
	mFont.Draw(500, 330, 8, 16, buf);

	//Y����]�l�̕����
	//������̐ݒ�
	sprintf(buf, "RY:%7.2f", mRotY);
	//�����̕`��
	mFont.Draw(500, 200, 8, 16, buf);

	//X����]�l�̕\��
	//������̐ݒ�
	sprintf(buf, "RX:%7.2f", mRotX);
	//������̕`��
	mFont.Draw(500, 300, 8, 16, buf);
	CCamera::End();  //2D�`��I��
}

void CUi::PosY(float f)
{
	mPosY = f;
}

void CUi::RotX(float f)
{
	mRotX = f;
}

void CUi::RotY(float f)
{
	mRotY = f;
}