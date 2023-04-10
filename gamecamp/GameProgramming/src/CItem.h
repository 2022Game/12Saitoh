#pragma once
#include "CCharacter.h"

class CItem : public CCharacter
{
private:
	static int mIh;
	static int sNum; //�A�C�e����
public:
	//�R���X�g���N�^
	CItem(float x, float y, float w, float h, CTexture* pt);
	static int Ih();
	//�A�C�e�������擾
	static int Num();

	//�Փˏ���4
	void Collision(CCharacter* m, CCharacter* o);

	//�X�V����
	void Update();
};