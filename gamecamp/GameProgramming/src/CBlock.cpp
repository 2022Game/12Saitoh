#include "CBlock.h"
#include "CApplication.h"

#define TEXCOORD 0, 0, 0, 0

//�R���X�g���N�^
CBlock::CBlock(float x, float y, float w, float h, CTexture* pt)
	:CCharacter((int)ETaskPriority::EBLOCK)
{
	Set(x, y, w, h*9);
	Texture(pt, TEXCOORD);
	mTag = ETag::EBLOCK;
}

//�Փ˔���2
void CBlock::Collision()
{
	//CApplication::CharacterManager()->Collision(this);
}

//�Փ˔���4
void CBlock::Collision(CCharacter* m, CCharacter* o)
{
}

//�X�V����
void CBlock::Update()
{
	CCharacter::Update();
}