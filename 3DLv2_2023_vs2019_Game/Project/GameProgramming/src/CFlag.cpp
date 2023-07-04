#include "CFlag.h"
#include "CCollisionManager.h"
#include "CEffect.h"
#include "CColliderMesh.h"

//コンストラクタ
//CFlag(モデル、位置、回転、拡縮）
CFlag::CFlag(CModel* model, const CVector& position,
	const CVector& rotation, const CVector& scale)
	: mCollider(this, &mMatrix, CVector(0.0f, 10.0f, 0.0f), 1.0f, CCollider::EColliderTag::EFLAG)
{
	//モデル、位置、回転、拡縮を設定する
	mpModel = model; //モデルの設定
	mPosition = position; //位置の設定
	mRotation = rotation; //回転の設定
	mScale = scale; //拡縮の設定
}

//更新処理
void CFlag::Update()
{
	CTransform::Update();
}

//衝突処理
//CCollision(コライダ１、コライダ２）
void CFlag::Collision(CCollider* m, CCollider* o)
{
	if (CCollider::Collision(m, o)) {
		if (o->ColliderTag() == CCollider::EColliderTag::EPLAYER)
		{
			mEnabled = false;
		}
	}
}

void CFlag::Collision()
{
	//コライダの優先度変更
	mCollider.ChangePriority();
	//衝突処理を実行
	CCollisionManager::Instance()->Collision(&mCollider, COLLISIONRANGE);}