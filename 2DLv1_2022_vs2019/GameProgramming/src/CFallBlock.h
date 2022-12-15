#pragma once
#include "CCharacter.h"
/*
CFallBlock
�����u���b�N�N���X
�}�b�v�̃u���b�N�ɂ��悤����
*/
class CFallBlock : public CCharacter
{
public:
	//��������l���擾
	int Fall();
	//�Փˏ���2
	void Collision();
	//�Փˏ���4
	void Collision(CCharacter* m, CCharacter* o);
	//CFallblock(X���W�AY���W�A���A�����A�e�N�X�`���̃|�C���^)
	CFallBlock(float x, float y, float w, float h, CTexture* pt);
	//�X�V����
	void Update();
private:
	int sfall;
};