#pragma once
#include "CCharacter.h"
#include "CInput.h"
#include "CMagicBullet.h"
#include "CSound.h"
/*
�v���C���[�N���X
�L�����N�^�X���p��
*/
class CPlayer : public CCharacter 
{
	friend CMagicBullet;
private:
	static CPlayer* spinstance;  //�v���C���[�̃C���X�^���X
	static int sHp;              //�v���C���[��HP
	static int sStamina;         //�v���C���[�̃X�^�~�i
	static int sCoolTime;        //�X�^�~�i�̃N�[���^�C��

	int mInvincible;             //���G�J�E���^

	float mJumpY;                //�v���C���[�̃W�����v����Y���W
	bool mDeath;                 //���S����
	CInput mInput;               //�L�[���͔���
	CSound mSoundRan;            //���鉹
	CMagicBullet* mpMagicBullet;

public:
	//�R���X�g���N�^
	//CPlayer(X���W,Y���W,��,����,�e�N�X�`���̃|�C���^)
	CPlayer(float x, float y, float w, float h, CTexture* pt);
	//~�f�X�g���N�^
	~CPlayer();

	//�C���X�^���X�̃|�C���^�̎擾
	static CPlayer* Instance();
	//�v���C���[��HP���擾
	static int HP();
	//�v���C���[�̃X�^�~�i���擾
	static int Stamina();

	//�Փˏ���
	void Collision(CCharacter* m, CCharacter* o);

	//�X�V����
	void Update();

	//�ړ�����
	void Move();
	//�W�����v����
	void Jump();
	//�A�C�h�����O����
	void Idling();
	//�U������
	void Attack();
	//��e����
	void Damage();
	//���S����
	void Death();
	//���S������擾
	bool BoolDeath();
};