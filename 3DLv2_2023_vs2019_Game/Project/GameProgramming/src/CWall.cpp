#include "CWall.h"
#include "CCollisionManager.h"
#include "CEffect.h"
#include "CColliderMesh.h"

//コンストラクタ
//CWall(モデル、位置、回転、拡縮）
CWall::CWall(CModel* model, const CVector& position,
	const CVector& rotation, const CVector& scale)
{
	//モデル、位置、回転、拡縮を設定する
	mpModel = model; //モデルの設定
	mPosition = position; //位置の設定
	mRotation = rotation; //回転の設定
	mScale = scale; //拡縮の設定
	mColliderMesh1.Set(this, &mMatrix, mpModel);
}

//更新処理
void CWall::Update()
{
	CTransform::Update();
}

//衝突処理
//CCollision(コライダ１、コライダ２）
void CWall::Collision(CCollider* m, CCollider* o) {
	if (CCollider::Collision(m, o)) {
		mEnabled = false;
	}
}

void CWall::Collision()
{
	//mColliderMesh1.ChangePriority();
	//コライダの優先度変更
	mCollider1.ChangePriority();
	mCollider2.ChangePriority();
	mCollider3.ChangePriority();
	//衝突処理を実行
	CCollisionManager::Instance()->Collision(&mCollider1, COLLISIONRANGE);
	CCollisionManager::Instance()->Collision(&mCollider2, COLLISIONRANGE);
	CCollisionManager::Instance()->Collision(&mCollider3, COLLISIONRANGE);
}