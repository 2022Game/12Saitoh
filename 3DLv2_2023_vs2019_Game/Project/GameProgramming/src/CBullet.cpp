#include "CBullet.h"
#include "CApplication.h"
#include "CVector.h"
#include <iostream>

#define M_PI 3.14159265358979323846
#define VELOCITYZ CVector(0.0f, 0.0f, 1.0f)	//Z軸移動
#define GRAVITY CVector(0.0f,-0.1f,0.0f) //重力


CBullet::CBullet()
	: mTime(50)
	, mCollider(this, &mMatrix, CVector(), 0.1f)
{

}

//幅と奥行きの設定
//Set(幅,奥行)
void CBullet::Set(float w, float d)
{
	//スケール設定
	mScale = CVector(1.0f, 1.0f, 1.0f);
	//三角形の頂点設定
	mT.Vertex(CVector(w, 0.0f, 0.0f), CVector(0.0f, 0.0f, -d), CVector(-w, 0.0f, 0.0f));
	//三角形の法線設定
	mT.Normal(CVector(0.0f, 1.0f, 0.0f));
}

//更新
void CBullet::Update()
{
	////生存時間の判定
	//if (mTime-- > 0)
	//{
	//	CTransform::Update();
	//	//位置更新
	//	mPosition = mPosition + VELOCITYZ * mMatrixRotate;
	//}
	//else
	//{
	//	//無効にする
	//	mEnabled = false;
	//}
}

//描画
void CBullet::Render()
{
	//DIFFUSE黄色設定
	float c[] = { 1.0f, 1.0f, 0.0f, 1.0f };
	glMaterialfv(GL_FRONT, GL_DIFFUSE, c);
	//三角形描画
	mT.Render(mMatrix);
}

void CBullet::Collision()
{
	//コライダの優先度変更
	mCollider.ChangePriority();
	//衝突処理を実行
	CCollisionManager::Instance()->Collision(&mCollider, COLLISIONRANGE);
}

//衝突処理
//Collider(コライダ1,コライダ2)
void CBullet::Collision(CCollider* m, CCollider* o)
{
	//コライダのmとoが衝突しているか判定
	if (CCollider::Collision(m, o))
	{
		//衝突している時は無効にする
		mEnabled = false;
	}
}