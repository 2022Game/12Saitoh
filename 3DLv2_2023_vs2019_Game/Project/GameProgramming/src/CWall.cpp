#include "CWall.h"
#include "CApplication.h"

#define SnowBall "res\\Wall.obj","res\\Wall.mtl"


CWall::CWall()
	: mCollider(this, &mMatrix, CVector(), 0.3f)
{
	mModelWall.Load(SnowBall);
}

//幅と奥行きの設定
//Set(幅,奥行)
void CWall::Set(float w, float d)
{
	//スケール設定
	mScale = CVector(0.3f, 0.3f, 0.3f);
}

//更新
void CWall::Update()
{
	CTransform::Update();
}

//描画
void CWall::Render()
{
	mModelWall.Render(mMatrix);
}

void CWall::Collision()
{
	//コライダの優先度変更
	mCollider.ChangePriority();
	//衝突処理を実行
	CCollisionManager::Instance()->Collision(&mCollider, COLLISIONRANGE);
}

//衝突処理
//Collider(コライダ1,コライダ2)
void CWall::Collision(CCollider* m, CCollider* o)
{
}
