#include"CEnemy.h"
#include"CApplication.h"

#define TEXCOORD 165, 351, 235, 127 //�e�N�X�`���}�b�s���O

//�R���X�g���N�^
CEnemy::CEnemy(float x, float y, float w, float h, CTexture* pt)
{
	Set(x, y, w*1.5, h);
	Texture(pt, TEXCOORD);
}

//�X�V����
void CEnemy::Update()
{

}