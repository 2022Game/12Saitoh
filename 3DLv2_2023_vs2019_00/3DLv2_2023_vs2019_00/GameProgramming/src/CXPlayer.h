#ifndef CXPLAYER
#define CXPLAYER

#include "CXCharacter.h"
#include "CInput.h"
#include "CCollider.h"
#include "CActionCamera.h"

class CXPlayer : public CXCharacter
{
private:
	CInput mInput;
	//�R���C�_�̐錾
	CCollider mColSpherHead;	//��
	CCollider mColSpherBody;	//��
	CCollider mColSpherSword;	//��
	//�J�����X�V����
	void UpdateCamera();

public:
	//�R���X�g���N�^
	CXPlayer();

	//�R���C�_�̐ݒ�
	void Init(CModelX* model);
	//�X�V����
	void Update();
};
#endif