#ifndef CENEMY3_H
#define CENEMY3_H
//キャラクタクラスのインクルード
#include "CCharacter3.h"
//コライダクラスのインクルード
#include "CCollider.h"
/*
エネミークラス
キャラクタクラスを継承
*/
class CEnemy3 : public CCharacter3
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
	CEnemy3();
	//CEnemy3(位置,回転,拡縮)
	CEnemy3(const CVector& position, const CVector& rotation,
		const CVector& scale);

	//更新処理
	void Update();

	void Collision();
	//衝突処理
	//Collision(コライダ1,コライダ2)
	void Collision(CCollider* m, CCollider* o);
};
#endif