#ifndef CCHARACTER_H
#define CCHARACTER_H
#include"CTask.h"
#include"CRectangle.h"
#include"CTexture.h"
/*
�L�����N�^�[�N���X
�Q�[���L�����N�^�̊�{�I�ȋ@�\���`����
*/
//�R���C�_�N���X�̐錾
class CCharacter : public CTask, public CRectangle
{
public:
	enum ETag
	{
		EZERO,          //�����l
		EPLAYER,        //�v���C���[
		EENEMY,         //�G
		EBOSS,          //�{�X
	};
protected:
	ETag mTag; //�^�O
private:
	CTexture* mpTexture;
	int mLeft, mRight, mBottom, mTop;
public:
	//�^�O�̎擾
	ETag Tag();
	//�R���X�g���N�^
	CCharacter(int priority);
	//�Փˏ���
	virtual void Collision(CCharacter* m, CCharacter* o) {}
	//�Փˏ���2
	virtual void Collision() {};
	//�R���X�g���N�^
	CCharacter();
	//�f�X�g���N�^
	~CCharacter();
	//�`�揈��
	void Render();
	CTexture* Texture();
	void Texture(CTexture* pTexture,
		int left, int right, int bottom, int top);
	bool Enabled();
};
#endif