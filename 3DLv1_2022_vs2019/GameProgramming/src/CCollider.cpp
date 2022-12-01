#include "CCollider.h"
#include "CCollisionManager.h"

CCollider::CCollider()
	:mpParent(nullptr)
	, mpMatrix(&mMatrix)
	, mType(ESPHERE)
	, mRadius(0)
{
	//コリジョンマネージャに追加
	CCollisionManager::Instance()->Add(this);
}
CCollider::CCollider(CCharacter3* parent, CMatrix* matrix,
	const CVector& position, float radius)
	:CCollider()
{
	//親設定
	mpParent = parent;
	//親行列設定
	mpMatrix = matrix;
	//CTransform設定
	mPosition = position;  //位置
	//半径設定
	mRadius = radius;
}

CCharacter3* CCollider::Parent()
{
	return mpParent;
}

void CCollider::Render()
{
	glPushMatrix();
	//コライダの座標×親の変換行列を掛ける
	CVector pos = mPosition * *mpMatrix;
	//中心座標へ移動
	glMultMatrixf(CMatrix().Translate(pos.X(), pos.Y(), pos.Z()).M());
	//DIFFUSE赤色設定
	float c[] = { 1.0f, 0.0f, 0.0f, 1.0f };
	glMaterialfv(GL_FRONT, GL_DIFFUSE, c);
	//球描画
	glutWireSphere(mRadius, 16, 16);
	glPopMatrix();
}

CCollider::~CCollider()
{
	//コリジョンリストから削除
	CCollisionManager::Instance()->Remove(this);
}

//衝突処理
//Collision(コライダ1,コライダ2)
//return:true(衝突している)false(衝突していない)
bool CCollider::Collision(CCollider* m, CCollider* o)
{
	//各コライダの中心座標を求める
	//原点×コライダの変換行列×親の変換行列
	CVector mpos = m->mPosition * *m->mpMatrix;
	CVector opos = o->mPosition * *o->mpMatrix;
	//中心から中心へベクトルを求める
	mpos = mpos - opos;
	//中心の距離が半径の合計より小さいと衝突
	if (m->mRadius + o->mRadius > mpos.Lenght())
	{
		//衝突している
		return true;
	}
	//衝突していない
	return false;
}