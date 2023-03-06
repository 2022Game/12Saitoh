#pragma once
#include "CUi.h"
#include "CEnemy.h"
#define TEXTURE "rafuimage.png" //�e�N�X�`���̃t�@�C����
#define BACKTEXTURE "backimage.png" //�w�i�e�N�X�`���̃t�@�C����
#define TIPSIZE 50
//�萔�̒�`
/*
* CGame�N���X
* �Q�[���N���X
*/
class CGame
{
public:
	//�f�X�g���N�^
	~CGame();
	//�Q�[���N���A����
	//bool IsClear();
	//�Q�[���N���A����
	void Clear();
	//�Q�[���I�[�o�[����
	//bool IsOver();
	//�Q�[���I�[�o�[����
	void Over();
	//�X�^�[�g����
	void Start();
	CGame(); //�f�t�H���g�R���X�g���N�^�i���������j
	void Update(); //�X�V����
private:
	CEnemy* mpEnemy;
	CEnemy* mpEnemy2;
	int mCdx, mCdy; //�J�����ƃv���C���[�̍��W�̍���
	void CameraSet(); //�J�����ݒ�
	int mTime; //�o�ߎ���
	CUi *mpUi; //UI�N���X�̃|�C���^
};
