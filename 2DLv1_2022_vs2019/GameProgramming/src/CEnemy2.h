#pragma once
#include"CCharacter.h"
#include"CApplication.h"

class CEnemy2 :public CCharacter
{
private:
	static int sNum;  //�G�̐�
public:
	//�G�̐��̏�����
	static int rNum();
	//�G�̐����擾
	static int Num();
	//�Փˏ���2
	void Collision();
	//�Փˏ���4
	void Collision(CCharacter* m, CCharacter* o);
	//CEnemy2(X���W,Y���W,��,����,�e�N�X�`���̃|�C���^)
	CEnemy2(float x, float y, float w, float h, CTexture* pt);
	//�X�V����
	void Update();
};