#ifndef CWALL
#define CWALL
#include "CCharacter3.h"
#include "CTriangle.h"
#include "CCollider.h"

class CWall : public CCharacter3
{
private:
	CCollider mCollider;
	CModel mModelWall;

public:
	CWall();
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