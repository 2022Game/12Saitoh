#ifndef CPALADIN_H
#define CPALADIN_H

#include "CXCharacter.h"

class CPaladin : public CXCharacter
{
private:
	static CModelX sModel;

public:
	//�R���X�g���N�^
	CPaladin();

	//�X�V����
	void Update();
};
#endif