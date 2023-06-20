#ifndef CCOLLISIONMANAGER_H
#define CCOLLISIONMANAGER_H
//衝突処理範囲より離れているコライダは衝突処理しない
#define COLLISIONRANGE 15

#include "CTaskManager.h"
#include "CCollider.h"
class CCollisionManager : public CTaskManager
{
public:
	void Collision(CCollider* c,int range);
	//衝突処理
	void Collision();
	//インスタンスの取得
	static CCollisionManager* Instance();
private:
	//マネージャのインスタンス
	static CCollisionManager* mpInstance;
};

#endif