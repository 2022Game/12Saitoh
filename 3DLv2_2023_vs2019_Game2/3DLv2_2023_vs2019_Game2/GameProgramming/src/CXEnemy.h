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
	CCollider mColSpherSword0;	//剣
	CCollider mColSpherSword1;	//剣
	CCollider mColSpherSword2;	//剣

public:
	//コンストラクタ
	CXEnemy();

	//コライダの設定
	void Init(CModelX* model);
	//更新処理
	void Update();

	//衝突処理
	void Collision(CCollider* m, CCollider* o);
};
#endif