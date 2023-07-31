#include <stdio.h>
#include "CUi.h"
#include "CCamera.h"
#include "CEnemy.h"
#include "CPlayer.h"

//�Q�[���V�[���̎擾
int CUi::Scene()
{
	return mScene = CSceneManager::Instance()->Scene();
}

CUi::CUi()
    : mScene(0)
	, mTime(0)
{
	//�|�[�Y�̎�ނ�ݒ�
	SetPauseType(TaskPauseType::eGame);
	mFont.Load("FontWhite.png", 1, 64);
	mPriority = (int)TaskPriority::eUI;
	//�^�X�N�}�l�[�W���Ɏ��M��ǉ�
	CTaskManager::Instance()->Add(this);
}

//�f�X�g���N�^
CUi::~CUi()
{
	CTaskManager::Instance()->Remove(this);
}

//�X�^�[�g���
void CUi::Title()
{
	CCamera::Start(0, 800, 0, 600); //2D�`��J�n
	mFont.Draw(36, 300, 18, 36, "START PUSH ENTER KEY!");
	CCamera::End();  //2D�`��I��
}

//�Q�[�����
void CUi::Game()
{
	CCamera::Start(0, 800, 0, 600);
	//2D�`��J�n
	//�`��F�̐ݒ�(�ΐF�̔�����)
	glColor4f(0.0f, 1.0f, 0.0f, 0.5f);
	//������ҏW�G���A�̍쐬
	char num[16];
	char hp[8];
	//������̐ݒ�
	sprintf(num, "NUM:%d", CEnemy::Instance()->Num());
	sprintf(hp, "HP:%d", CPlayer::Instance()->HP());
	//�����̕`��
	mFont.Draw(700, 550, 10, 20, num);
	mFont.Draw(700, 500, 10, 20, hp);
	CCamera::End();  //2D�`��I��
}

//�Q�[���I�[�o�[�\��
void CUi::Over()
{
	CCamera::Start(0, 800, 0, 600); //2D�`��J�n
	mFont.Draw(250, 300, 18, 36, "GAME OVER!!");
	CCamera::End();  //2D�`��I��
}

//�Q�[���N���A�\��
void CUi::Clear()
{
	CCamera::Start(0, 800, 0, 600);
	mFont.Draw(250, 300, 18, 36, "GAME CLEAR!!");
	CCamera::End();
}

//�`�揈��
void CUi::Render()
{
	switch (Scene())
	{
	case 1:	//�^�C�g���V�[��
		Title();
		break;
	case 2:	//�Q�[���V�[��
		Game();
		break;
	case 3:	//�Q�[���I�[�o�[�V�[��
		Over();
		break;
	case 4:	//�Q�[���N���A�V�[��
		Clear();
		break;
	default://�ǂ̃V�[���ɂ��������Ȃ�
		break;
	}
}

//�o�ߎ��Ԃ̎擾
void CUi::Time(int time)
{
	mTime = time;
}