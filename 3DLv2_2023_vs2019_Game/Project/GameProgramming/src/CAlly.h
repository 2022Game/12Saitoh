#ifndef ALLY_H
#define ALL_Y
#include "CCharacter3.h"
#include "CCollider.h"
/*
味方クラス
*/
class CAlly : public CCharacter3
{
private:
	CVector mPoint;			//目標地点
	CCollider mCollider1;	//頭コライダ
	CCollider mCollider2;	//胴体コライダ
	CCollider mCollider3;	//下半身コライダ

	int mHp;				//ヒットポイント

	static CModel sModel;	//モデルデータ

public:
	//コンストラクタ
	CAlly();
	//CEnemy3(位置,回転,拡縮)
	CAlly(const CVector& position, const CVector& rotation,
		const CVector& scale);

	//更新処理
	void Update();

	void Collision();
	//衝突処理
	//Collision(コライダ1,コライダ2)
	void Collision(CCollider* m, CCollider* o);
};
#endif