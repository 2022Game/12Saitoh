#pragma once
#include "CCharacter.h"
#include "CCharacterManager.h"

class CBackGround : public CCharacter
{
private:
	/*
	�w�i���ړ���������
	true:�ړ���
	false:��~��
	*/
	static bool mMoveStage;
public:
	//�R���X�g���N�^
	CBackGround::CBackGround(float x, float y, float w, float h, float l, float r, float b, float t, CTexture* pt);
	//�X�V����
	void Update();
	//�w�i�̏�Ԃ��擾
	static bool GetMoveStage();
};