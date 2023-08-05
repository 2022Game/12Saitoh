#ifndef CCHARACTER3_H
#define CCHARACTER3_H
//�ϊ��s��N���X�̃C���N���[�h
#include "CTransform.h"
//���f���N���X�̃C���N���[�h
#include "CModel.h"
#include "CTask.h"
//�R���C�_�N���X�̐錾
class CCollider;

/*
�L�����N�^�[�N���X
�Q�[���L�����N�^�̊�{�I�ȋ@�\���`����
*/
class CCharacter : public CTransform, public CTask {
public:
	enum ETag
	{
		EZERO,			//�����l
		EPLAYER,		//�v���C���[
		EENEMY,			//�G
		EBULLETPLAYER,	//�v���C���[�e
		EBULLETENEMY,	//�G�e
	};
	//�^�O�̎擾
	ETag Tag();

	//�Փˏ���
	virtual void Collision(CCollider* m, CCollider* o) {}
	//�R���X�g���N�^
	CCharacter();
	//�R���X�g���N�^
	CCharacter(int priority);
	//�f�X�g���N�^
	~CCharacter();
	//���f���̐ݒ�
	//Model(���f���N���X�̃|�C���^)
	void Model(CModel* m);
	//�`�揈��
	void Render();
protected:
	ETag mTag;	//�^�O
	CModel* mpModel; //���f���̃|�C���^
};

#endif

