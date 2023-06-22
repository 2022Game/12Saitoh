#ifndef CXENEMY_H
#define CXENEMY_H

#include "CXCharacter.h"
#include "CCollider.h"

class CXEnemy : public CXCharacter
{
private:
	//コライダの宣言
	CCollider mColSpherHead;	//頭
	CCollider mColSpherBody;	//体
	CCollider mColSpherSword;	//剣

public:
	//コンストラクタ
	CXEnemy();

	//コライダの設定
	void Init(CModelX* model);
	//更新処理
	void Update();
};
#endif