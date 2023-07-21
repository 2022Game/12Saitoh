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
	//コライダの宣言
	CCollider mColSpherHead;	//頭
	CCollider mColSpherBody;	//体
	CCollider mColSpherSword;	//剣

public:
	//コンストラクタ
	CXPlayer();

	//コライダの設定
	void Init(CModelX* model);
	//更新処理
	void Update();
};
#endif