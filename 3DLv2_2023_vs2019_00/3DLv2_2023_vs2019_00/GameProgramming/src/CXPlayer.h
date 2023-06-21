#ifndef CXPLAYER
#define CXPLAYER

#include "CXCharacter.h"
#include "CInput.h"
#include "CCollider.h"

class CXPlayer : public CXCharacter
{
private:
	CInput mInput;
	//コライダの宣言
	CCollider mColSpherBody;	//体

public:
	//コンストラクタ
	CXPlayer();

	void Init(CModelX* model);
	//更新処理
	void Update();
};
#endif