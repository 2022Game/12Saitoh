#include "CGame.h"
#include "CApplication.h"
#include "CCamera.h"
#include "main.h"

CGame::CGame()
	: mpUi(nullptr)
	, mTime(0)
	, mCdx(0)
	, mCdy(0)
{	
	//�e�N�X�`���̓���
	CApplication::Texture()->Load(TEXTURE);
	//�w�i�e�N�X�`���̓���
	CApplication::Texture2()->Load(BACKTEXTURE);
	mpUi = new CUi();
	//�w�i���L�����N�^�}�l�[�W���ɒǉ�����
	CApplication::CharacterManager()->Add(
		new CBack(640, 400, 1200, 400, CApplication::Texture2()));
	//�v���C���[���L�����N�^�}�l�[�W���ɒǉ�����
	CApplication::CharacterManager()->Add(
		new CPlayer(TIPSIZE * 10, TIPSIZE * 5, TIPSIZE, TIPSIZE, CApplication::Texture()));
	//�G���L�����N�^�}�l�[�W���ɒǉ�����
	CApplication::CharacterManager()->Add(
		mpEnemy = new CEnemy(TIPSIZE * 15, TIPSIZE * 4.5, TIPSIZE, TIPSIZE, CApplication::Texture()));
	CApplication::CharacterManager()->Add(
		mpEnemy2 = new CEnemy(TIPSIZE * 20, TIPSIZE * 7, TIPSIZE, TIPSIZE, CApplication::Texture()));
}

void CGame::Start()
{
	//�Q�[���̕`��
	CApplication::CharacterManager()->Render();
	//UI����
	mpUi->Render();
	mpUi->Start();
}

//bool CGame::IsOver()
//{
//
//}

void CGame::Over()
{
	CameraSet();
	//�Q�[���̕`��
	CApplication::CharacterManager()->Render();
	CCamera::End();
	//UI����
	mpUi->Render();
	mpUi->Over();
}

CGame::~CGame()
{
	//�S�ẴC���X�^���X�폜
	CApplication::CharacterManager()->AllDelete();
	//UI�𐶐����Ă��鎞
	if (mpUi != nullptr)
	{	//UI���폜���A������
		delete mpUi;
		mpUi = nullptr;
	}
}

//bool CGame::IsClear()
//{
//
//}

void CGame::Clear()
{
	CameraSet();
	//�Q�[���̕`��
	CApplication::CharacterManager()->Render();
	CCamera::End();
	//UI����
	mpUi->Render();
	mpUi->Clear();
}

void CGame::Update()
{
	//�X�V�A�ՓˁA�폜�A�`��
	CApplication::CharacterManager()->Update();
	CApplication::CharacterManager()->Collision();
	CApplication::CharacterManager()->Delete();
	CameraSet();
	CApplication::CharacterManager()->Render();
	CCamera::End();
	//UI
	mpUi->Time(mTime++);
	mpUi->Render();
}

void CGame::CameraSet()
{

}
