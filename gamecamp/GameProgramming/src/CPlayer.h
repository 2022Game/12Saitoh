#ifndef CPLAYER_H
#define CPLAYER_H
//�L�����N�^�X�N���X�̃C���N���[�h
#include "CCharacter.h"
#include "CInput.h"
/*
�v���C���[�N���X
�L�����N�^�X���p��
*/
class CPlayer : public CCharacter
{
public:
	//�C���X�^���X�̃|�C���^�̎擾
	static CPlayer* Instance();
	//�R���X�g���N�^
	//CPlayer(X���W,Y���W,��,����,�e�N�X�`���̃|�C���^)
	CPlayer(float x, float y, float w, float h, CTexture* pt);
	//�X�V����
	void Update();
private:
	//�v���C���[�̃C���X�^���X
	static CPlayer* spInstance;
	CInput mInput;
};
#endif