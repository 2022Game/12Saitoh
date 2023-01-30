#pragma once
#include "CRectangle.h"
#include "CTexture.h"
#include <stdio.h>

#define VELOCITY 4.0f	//�ړ����x

class CCharacter : public CRectangle
{
public:
	enum class ETag
	{
		EZERO,		//�����l
		EBULLET,	//�e
		EENEMY,		//�G
		EENEMY2,    //�G2
		EPLAYER,	//�v���C���[
		ETURN,		//�܂�Ԃ�
		EBLOCK,		//�u���b�N
		ETRAP,      //�g���b�v
		EWALL,      //�Q�[���I�[�o�[����p�̕�
		EGOAL,      //�S�[��
		EFOLLBLOCK, //������
		EDOKAN,     //�y��
		EBACK,      //�w�i
	};
	enum class EState	//���
	{
		EMOVE,	//�ړ�
		ESTOP,	//��~
		EJUMP,	//�W�����v
		ECRY,	//����
		EFOLL,  //����
	};
	enum class Espstate  //�B���X�e�C�^�X
	{
		ESPMOVE,    //�ړ�
		EHIGHJUMP,  //�n�C�W�����v 
		NULLSTATE,  //��
	};
protected:
	float mVx;	//X�����x
	float mVy;	//Y�����x
	bool mEnabled;
	ETag mTag;
	EState mState;
	Espstate mspState;
private:
	CTexture *mpTexture;
	int mLeft, mRight, mBottom, mTop;
public:
	Espstate spState(); //�B���X�e�[�^�X���擾����
	EState State();	//��Ԃ��擾����
	bool Enabled();
	ETag Tag();
	//�Փˏ����Q
	virtual void Collision() {};
	//�Փˏ����S
	//Collision(�����̃|�C���^, �Փˑ���̃|�C���^)
	virtual void Collision(CCharacter* m, CCharacter* o) {};
	virtual void Update() = 0;
	CCharacter();
	CTexture* Texture();
	void Texture(CTexture *pTexture, int left, int right, int bottom, int top);
	virtual void Render();
	void Move();
};