#include"CBack.h"
#include"CApplication.h"

#define TEXCOORD 0, 1115, 581, 0 //�e�N�X�`���}�b�s���O
//�R���X�g���N�^
CBack::CBack(float x, float y, float w, float h, CTexture* pt)
{
	Set(x, y, w , h);
	Texture(pt, TEXCOORD);
}