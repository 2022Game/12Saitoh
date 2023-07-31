#include "CPlayerBullet.h"
#include "CApplication.h"
#include <cmath>
#include "CCollisionManager.h"


#define VELOCITYZ CVector(0.0f, 0.0f, 1.5f)	//Z軸移動
#define GRAVITY CVector(0.0f,-0.1f,0.0f) //重力
#define SnowBall "res\\SnowBall.obj","res\\SnowBall.mtl"

CPlayerBullet* CPlayerBullet::spinstence = nullptr;

//コンストラクタ
CPlayerBullet::CPlayerBullet(CCollider::EColliderTag tag)
	: mCollider(this, &mMatrix, CVector(), 0.3f,tag)
	, mV(0.0f,0.0f,0.0f)

{
	mModelBall.Load(SnowBall);
	spinstence = this;
}

//幅と奥行きの設定
//Set(幅,奥行)
void CPlayerBullet::Set(float w, float d)
{
	//スケール設定
	mScale = CVector(0.3f, 0.3f, 0.3f);
	//三角形の頂点設定
	//mT.Vertex(CVector(w, 0.0f, 0.0f), CVector(0.0f, 0.0f, -d), CVector(-w, 0.0f, 0.0f));
	//三角形の法線設定
	//mT.Normal(CVector(0.0f, 1.0f, 0.0f));
}

//更新
void CPlayerBullet::Update()
{
	CTransform::Update();
	//位置更新
	mPosition = mPosition + (VELOCITYZ * mMatrixRotate) + (mV * mMatrixRotate);
	mV = mV + CVector(0.0f, -0.01f, 0.0f);
}

//描画
void CPlayerBullet::Render()
{
	mModelBall.Render(mMatrix);
}

void CPlayerBullet::Collision()
{
	//コライダの優先度変更
	mCollider.ChangePriority();
	//衝突処理を実行
	CCollisionManager::Instance()->Collision(&mCollider, COLLISIONRANGE);
}

//衝突処理
//Collider(コライダ1,コライダ2)
void CPlayerBullet::Collision(CCollider* m, CCollider* o)
{
	//相手のコライダタイプの判定
	switch (o->Type())
	{
	case CCollider::ETRIANGLE: //三角コライダの時
	{
		CVector adjust; //調整値
		//三角コライダと球コライダの衝突判定
		if (CCollider::CollisionTriangleSphere(o, m, &adjust))
		{
			mEnabled = false;
		}
	}
	break;
	case CCollider::ESPHERE: //球コライダの時
		//相手が敵の時
		if (o->ColliderTag() == CCollider::EColliderTag::EENEMY)
		{
			if (CCollider::Collision(m, o))
			{
				mEnabled = false;
			}
		}
		break;
	}
}

CPlayerBullet* CPlayerBullet::Instance()
{
	return spinstence;
}