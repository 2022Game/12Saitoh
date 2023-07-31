#include "CAlly.h"
#include "CEffect.h"
#include "CApplication.h"
#include "CPlayer.h"
#include "CCollisionManager.h"

#define HP 3						//耐久値
#define VELOCITY 0.1f				//速度
#define OBJ "res\\SnowGolem.obj"	//モデルのファイル
#define MTL "res\\SnowGolem.mtl"	//モデルのマテリアルファイル
#define GRAVITY CVector(0.0f, 0.1f, 0.0f)	//重力


CModel CAlly::sModel;				//モデルデータ作成

//デフォルトコンストラクタ
CAlly::CAlly()
	:CCharacter3(1)
	, mCollider1(this, &mMatrix, CVector(0.0f, 70.0f, 0.0f), 0.4f, CCollider::EColliderTag::EALL)
	, mCollider2(this, &mMatrix, CVector(0.0f, 45.0f, 0.0f), 0.36f, CCollider::EColliderTag::EALL)
	, mCollider3(this, &mMatrix, CVector(0.0f, 13.0f, 0.0f), 0.45f, CCollider::EColliderTag::EALL)
	, mHp(HP)
{
	//モデルが無い時は読み込む
	if (sModel.Triangles().size() == 0)
	{
		sModel.Load(OBJ, MTL);
	}
	//モデルデータのポインタ設定
	mpModel = &sModel;
}

//コンストラクタ
//CAlly(位置,回転,拡縮)
CAlly::CAlly(const CVector& position, const CVector& rotation,
	const CVector& scale)
	:CAlly() //デフォルトコンストラクタを実行する
{
	//位置,回転,拡縮を設定する
	mPosition = position; //位置の設定
	mRotation = rotation; //回転の設定
	mScale = scale;       //拡縮の設定
	CTransform::Update(); //行列の更新
	//目標地点の設定
	mPoint = mPosition + CVector(0.0f, 0.0f, 100.0f) * mMatrixRotate;
}

//更新処理
void CAlly::Update()
{
	mPosition = mPosition - GRAVITY;
	CTransform::Update();
}


void CAlly::Collision()
{
	//コライダの優先度変更
	mCollider1.ChangePriority();
	mCollider2.ChangePriority();
	mCollider3.ChangePriority();
	//衝突処理を実行
	CCollisionManager::Instance()->Collision(&mCollider1, COLLISIONRANGE);
	CCollisionManager::Instance()->Collision(&mCollider2, COLLISIONRANGE);
	CCollisionManager::Instance()->Collision(&mCollider3, COLLISIONRANGE);
}

//衝突処理
//Collision(コライダ1,コライダ2)
void CAlly::Collision(CCollider* m, CCollider* o)
{
	//相手のコライダタイプの判定
	switch (o->Type())
	{
	case CCollider::ESPHERE: //球コライダの時
		//コライダのmとyが衝突しているか判定
		if (CCollider::Collision(m, o))
		{
			if (o->ColliderTag() == CCollider::EColliderTag::EENEMYBULLET)
			{
				mHp--; //ヒットポイントの減算
				//エフェクト生成
				new CEffect(o->Parent()->Position(), 1.0f, 1.0f, "exp.tga", 4, 4, 2);
			}
		}
		break;
	case CCollider::ETRIANGLE: //三角コライダの時
		CVector adjust; //調整値
		//三角コライダと球コライダの衝突判定
		if (CCollider::CollisionTriangleSphere(o, m, &adjust))
		{
			//衝突していない位置まで戻す
			mPosition = mPosition + adjust;
			//撃破で地面に衝突すると無効
			if (mHp <= 0)
			{
				mEnabled = false;
			}
		}
		break;
	}
}