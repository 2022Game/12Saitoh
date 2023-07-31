#ifndef CCHARACTER3_H
#define CCHARACTER3_H
//�ϊ��s��N���X�̃C���N���[�h
#include "CTransform.h"
//���f���N���X�̃C���N���[�h
#include "CModel.h"
#include "CTaskManager.h"
#include "CTask.h"
//�R���C�_�N���X�̐錾
class CCollider;
class NavNode;

/*
�L�����N�^�[�N���X
�Q�[���L�����N�^�̊�{�I�ȋ@�\���`����
*/
class CCharacter3 : public CTransform, public CTask
{
public:
	enum ETag
	{
		EZERO,          //�����l
		EPLAYER,        //�v���C���[
		EENEMY,         //�G
	};
	//�^�O�̎擾
	ETag Tag();
	//�R���X�g���N�^
	CCharacter3();
	//�R���X�g���N�^
	CCharacter3(int priority);
	//�f�X�g���N�^
	~CCharacter3();

	//���f���̐ݒ�
	//Model(���f���N���X�̃|�C���^)
	void Model(CModel* m);
	//�Փˏ���
	virtual void Collision(CCollider* m, CCollider* o) {}
	//�X�V����
	virtual void Update();
	//�`�揈��
	virtual void Render();

	NavNode* GetNavNode() const;

	//�L�����N�^�[������ł��邩�ǂ���
	bool IsDeath();

	//HP���擾
	int HP() const;
protected:
	int mHp;			//HP

	ETag mTag;			//�^�O
	CVector mGravity;	//�d��
	CModel* mpModel;	//���f���̃|�C���^
	NavNode* mpNode;	//���g�̍��W�̃m�[�h
};
#endif