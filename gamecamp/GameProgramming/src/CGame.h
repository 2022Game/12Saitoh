#pragma once
#include "CUi.h"
#include "CPlayer.h"
#include "CEnemy.h"
#include "CEnemy2.h"
#include "CBackGround.h"
#include "CBackGround2.h"
#include "CBackGround3.h"
#include "CBackGround4.h"
#include "CHeart.h"
#include "CInput.h"
#include "CSlime.h"
#include "CWolf.h"
#include "CBoss.h"
#include "CItem.h"
#include "CBlock.h"

//�萔�̒�`
#define TEXTURE "image.png"//�e�N�X�`���̃t�@�C����
#define TIPSIZE 40.0f //��{�T�C�Y
class CGame
{
private:
	CPlayer* mpPlayer; //�v���C���[�̃|�C���^
	CEnemy* mpEnemy;   //�G�̃|�C���^
	CEnemy2* mpEnemy2; //�G�Q�̃|�C���^
	CSlime* mpSlime;
	CWolf* mpWolf;
	CBoss* mpBoss;
	CItem* mpItem;
	CBlock* mpBlock;
	CBackGround* mpBackGround;
	CBackGround2* mpBackGround2;
	CBackGround3* mpBackGround3;
	CBackGround4* mpBackGround4;
	CHeart* mpHeart;
	CInput mInput;
	CUi* mpUi;

	int mCdx, mCdy;      //�J�����ƃv���C���[�̍��W�̍���
	int mTime;           //�o�ߎ���
	static int sNum;     //�G�̐�
	static int sItemNum; //�A�C�e���̐�

public:
	//�A�C�e���̐����擾
	static int ItemuNum();
	//�G�̐����擾
	static int Num();

	//�R���X�g���N�^
	CGame();
	//�f�X�g���N�^
	~CGame();

	//�Q�[���N���A����
	void Clear();

	//�Q�[���I�[�o�[����
	void Over();

	//�X�^�[�g����
	void Start();
	//�X�e�[�W
	void Stage1();
	void Stage2();
	void Boss();
	void SetItem();
	//�X�V����
	void Update();
};
