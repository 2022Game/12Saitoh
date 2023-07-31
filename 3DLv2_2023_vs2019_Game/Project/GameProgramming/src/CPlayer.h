#ifndef PLAYER_H
#define PLAYER_H
//�L�����N�^�X�N���X�̃C���N���[�h
#include "CCharacter3.h"
#include "CInput.h"
#include "CPlayerBullet.h"
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

	CPlayerBullet* mpBullet;
	CCollider mCollider1;	//���R���C�_
	CCollider mCollider2;	//���̃R���C�_
	CCollider mCollider3;	//�����g�R���C�_
	CInput mInput;

public:
	//�R���X�g���N�^
	CPlayer();
	//CPlayer(�ʒu,��],�X�P�[��)
	CPlayer(const CVector& pos, const CVector& rot, const CVector& scale);
	//�f�X�g���N�^
	~CPlayer();

	//�X�V����
	void Update();

	void Collision();
	//�Փˏ���
	void Collision(CCollider* o, CCollider* m);

	//�C���X�^���X�̃|�C���^�̎擾
	static CPlayer* Instance();
};
#endif