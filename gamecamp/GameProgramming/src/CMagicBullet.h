#pragma once
#include "CCharacter.h"

class CMagicBullet : public CCharacter
{
private:
	int mDc;
public:
	//�R���X�g���N�^
	CMagicBullet(float x, float y, float w, float h, CTexture* pt);
	//�f�X�g���N�^
	~CMagicBullet();

	//�Փˏ���
	void Collision(CCharacter* m, CCharacter* o);

	//�X�V����
	void Update();

};