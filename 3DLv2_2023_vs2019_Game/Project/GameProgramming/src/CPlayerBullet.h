#ifndef CPLAYERBULLET_H
#define CPLAYERBULLET_H
//キャラクタクラスのインクルード
#include "CCharacter3.h"
//三角形クラスのインクルード
#include "CTriangle.h"
//コライダクラスのインクルード
#include "CCollider.h"
#include "CEnemy.h"
/*
弾クラス
三角形を飛ばす
*/
class CPlayerBullet : public CCharacter3 
{
	friend CEnemy;
private:
	CCollider mCollider;
	CVector mV;	//水平投射用
	CModel mModelBall;
	static CPlayerBullet* spinstence;

public:
	//コンストラクタ
	CPlayerBullet(CCollider::EColliderTag tag);
	//幅と奥行きの設定
	//Set(幅,奥行)
	void Set(float w, float d);
	//更新
	void Update();
	//描画
	void Render();
	void Collision();
	//衝突処理
	//Collision(コライダ1,コライダ2)
	void Collision(CCollider* m, CCollider* o);
	static CPlayerBullet* Instance();
};
#endif;