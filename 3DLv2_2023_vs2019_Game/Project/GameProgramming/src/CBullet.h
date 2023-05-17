#ifndef CBULLET_H
#define CBULLET_H
//キャラクタクラスのインクルード
#include "CCharacter3.h"
//三角形クラスのインクルード
#include "CTriangle.h"
//コライダクラスのインクルード
#include "CCollider.h"
/*
弾クラス
三角形を飛ばす
*/
class CBullet : public CCharacter3
{
private:
	CCollider mCollider;
	CTriangle mT;	//三角形
	int mTime;		//経過時間(フレーム)

public:
	CBullet();
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
};
#endif;