#ifndef CENEMY_H
#define CENEMY_H
//�L�����N�^�N���X�̃C���N���[�h
#include "CCharacter.h"
/*
�G�l�~�[�N���X
�L�����N�^�N���X���p��
*/
class CEnemy : public CCharacter
{
public:
	//�R���X�g���N�^
	//CEnemy(X���W,Y���W,��,����,�e�N�X�`���̃|�C���^)
	CEnemy(float x, float y, float w, float h, CTexture* pt);
	void Update();
};
#endif