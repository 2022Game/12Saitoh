#ifndef CXPLAYER
#define CXPLAYER

#include "CXCharacter.h"
#include "CInput.h"
#include "CCollider.h"

class CXPlayer : public CXCharacter
{
private:
	CInput mInput;
	//�R���C�_�̐錾
	CCollider mColSpherBody;	//��

public:
	//�R���X�g���N�^
	CXPlayer();

	void Init(CModelX* model);
	//�X�V����
	void Update();
};
#endif