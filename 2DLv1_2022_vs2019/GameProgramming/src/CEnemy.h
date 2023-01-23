#pragma once
#include"CCharacter.h"

class CEnemy :public CCharacter
{
public:
	static CEnemy* Instance();
	//�Փˏ���2
	void Collision();
	//�Փˏ���4
	void Collision(CCharacter* m, CCharacter* o);
	//CEnemy2(X���W,Y���W,��,����,�e�N�X�`���̃|�C���^)
	CEnemy(float x, float y, float w, float h, CTexture* pt);
	//�X�V����
	void Update();
private:
	static CEnemy* spInstance;
	int cooltime;
};