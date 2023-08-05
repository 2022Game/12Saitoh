#ifndef CPALADIN_H
#define CPALADIN_H

#include "CXCharacter.h"

class CPaladin : public CXCharacter
{
private:
	static CModelX sModel;

public:
	//コンストラクタ
	CPaladin();

	//更新処理
	void Update();
};
#endif