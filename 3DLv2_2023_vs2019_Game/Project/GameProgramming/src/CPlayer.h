#ifndef CPLAYER_H
#define CPLAYER_H
//�L�����N�^�X�N���X�̃C���N���[�h
#include "CCharacter3.h"
#include "CInput.h"
#include "CBullet.h"
#include "CColliderLine.h"
/*
�v���C���[�N���X
�L�����N�^�X���p��
*/
class CPlayer : public CCharacter3
{
private:
	//�v���C���[�̃C���X�^���X
	static CPlayer* spInstance;
	CCollider mCollider1;	//���R���C�_
	CCollider mCollider2;	//���̃R���C�_
	CCollider mCollider3;	//�����g�R���C�_
	CInput mInput;


public:
	//�C���X�^���X�̃|�C���^�̎擾
	static CPlayer* Instance();

	CPlayer();
	//CPlayer(�ʒu,��],�X�P�[��)
	CPlayer(const CVector& pos, const CVector& rot, const CVector& scale);
	//�X�V����
	void Update();

	void Collision();
	//�Փˏ���
	void Collision(CCollider* o, CCollider* m);
};
#endif