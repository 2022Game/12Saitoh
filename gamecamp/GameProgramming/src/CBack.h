#ifndef CBACK_H
#define CBACK_H
//�L�����N�^�N���X�̃C���N���[�h
#include "CCharacter.h"
/*
�w�i�N���X
�L�����N�^�N���X���p��
*/
class CBack : public CCharacter
{
public:
	//�R���X�g���N�^
	//CBack(X���W,Y���W,��,����,�e�N�X�`���̃|�C���^)
	CBack(float x, float y, float w, float h, CTexture* pt);
};
#endif