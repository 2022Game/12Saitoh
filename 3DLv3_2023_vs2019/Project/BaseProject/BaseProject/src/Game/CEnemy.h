#ifndef CENEMY_H
#define CENEMY_H
#include "CXCharacter.h"
#include "CCollider.h"
#include "CModel.h"

/*
�G�l�~�[�N���X
�L�����N�^�N���X���p��
*/
class CEnemy : public CXCharacter
{
public:
	// �C���X�^���X���擾
	static CEnemy* Instance();
	// �R���X�g���N�^
	CEnemy();

	// �X�V����
	void Update();

private:
	static CEnemy* spInstance;
	CModel* mModel;
};

#endif
