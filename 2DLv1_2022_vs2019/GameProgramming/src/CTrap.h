#pragma once
#include "CCharacter.h"
#include"CPlayer2.h"
/*
�����u���b�N�N���X
*/

class CTrap : public CCharacter
{
public:
	//�Փˏ���2
	void Collision();
	//�Փˏ���4
	void Collision(CCharacter* m, CCharacter* o);
	//CTrap(X���W,Y���W,��,����,�e�N�X�`���̃|�C���^)
	CTrap(float x, float y, float w, float h, CTexture* pt);
	//�X�V����
	void Update(){};
};