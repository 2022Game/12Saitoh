#pragma once
#include "CCharacter.h"
#include "CInput.h"
#include "CSound.h"

class CPlayer2 : public CCharacter
{
public:
	static CPlayer2* Instance();
	//�S�[���̒l���擾
	static int Goal();
	//HP���擾
	static int Hp();
	//�Փˏ���2
	void Collision();
	//�Փˏ���4
	void Collision(CCharacter* m, CCharacter* o);
	//CPlayer2(X���W,Y���W,��,����,�e�N�X�`���̃|�C���^)
	CPlayer2(float x, float y, float w, float h, CTexture* pt);
	//�X�V����
	void Update();
private:
	static int sgoal;
	static CPlayer2* spInstance;
	int interval; //�W�����v�̃C���^�[�o��
	CSound mSoundJump;	//�W�����vSE
	CSound mSoundDown;  //���݂�SE
	static int sHp;	//HP
	int mInvincible; //���G�J�E���^
	CInput mInput;
};