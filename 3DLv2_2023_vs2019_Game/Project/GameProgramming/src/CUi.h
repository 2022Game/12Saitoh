#pragma once
#include "CFont.h"
#include "CTask.h"
#include "CSceneManager.h"
/*
* CUi
* ���[�U�[�C���^�t�F�[�X�N���X
*/
class CUi : public CTask
{
private:
	int Scene();//�Q�[���V�[���̎擾
	int mScene;	//�V�[��
	int mTime;	//�o�ߎ���
	CFont mFont;

public:
	//�R���X�g���N�^
	CUi();
	//�f�X�g���N�^
	~CUi();

	//�X�^�[�g�\��
	void Title();
	//�Q�[�����
	void Game();
	//�Q�[���I�[�o�[�\��
	void Over();
	//�Q�[���N���A�\��
	void Clear();
	//�`�揈��
	void Render();

	//�o�ߎ��Ԃ̎擾
	void Time(int time);
};
