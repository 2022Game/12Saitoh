#pragma once
#include "CCharacter.h"
class CBlackBlock : public CCharacter
{
public:
	//CBlock(X���W,Y���W,��,����,�e�N�X�`���̃|�C���^)
	CBlackBlock(float x, float y, float w, float h, CTexture* pt);
	//Update�ł͉������Ȃ�
	void Update() {}
};