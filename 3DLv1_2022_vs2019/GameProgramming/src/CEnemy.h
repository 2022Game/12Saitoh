#ifndef CENEMY_H
#define CENEMY_H
//キャラクタクラスのインクルード
#include "CCharacter3.h"
//コライダクラスのインクルード
#include "CCollider.h"
/*
エネミークラス
キャラクタクラスを継承
*/
class CEnemy : public CCharacter3
{
private:
	//コライダ
	CCollider mCollider1;
	CCollider mCollider2;
	CCollider mCollider3;
public:
	//コンストラクタ
	//CEnemy(モデル、位置、回転、拡縮)
	CEnemy(CModel* model, const CVector& position,
		const CVector& rotation, const CVector& scale);
	//更新処理
	void Update();
};
#endif